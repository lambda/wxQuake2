#ifdef IML_Q2_EXTENSIONS

#include <limits.h>
#include "r_local.h"

#define MAX_OVERLAYS (128)
#define ICLUT_BITS   (5)
#define IGNORED_BITS (8 - ICLUT_BITS)
#define OPAQUE_PALETTE_ENTRIES (255) /* 256 is transparent */

/* The data we need to know about each overlay. */
struct overlay_s {
    int format;
    byte *raw_data;
    size_t left, top, width, height;
    int stride;
    int is_shown;

    /* A copy of raw_data which has been converted to 8-bit data.  The
    ** portion marked as dirty needs to be re-converted before display. */
    byte *indexed_data;

    /* Dirty rectangle. The right and bottom co-ordinates are
    ** exclusive. */
    size_t dl, dt, dr, db;
};

/* We keep a list of all active overlays so we can draw them. */
static size_t gOverlayCount = 0;
overlay_t *gOverlays[MAX_OVERLAYS];

/* This is an inverse color lookup table, which maps RGB values to palette
** indices in a relatively efficient manner. It only uses the ICLUT_BITS
** most significant bits of each color. */
byte gInverseCLUT[1 << 3*ICLUT_BITS];

/* Look up an appropriate ICLUT entry.*/
#define ICLUT(R,G,B) \
  (gInverseCLUT[((R>>IGNORED_BITS) << 2*ICLUT_BITS) | \
                ((G>>IGNORED_BITS) << 1*ICLUT_BITS) | \
                ((B>>IGNORED_BITS))])

/* We have 1<<ICLUT_BITS entries for each channel, which we want to spread
** evenly between 0 and 255, inclusive.  Multiply a 5-bit channel by this
** value to get an evenly-distributed 8-bit value. */
#define ICLUT_SCALE (255.0 / ((1<<ICLUT_BITS)-1)) 

void R_InitOverlays(byte *palette)
{
    int r, g, b, i;
    int r_dist, g_dist, b_dist, dist, best_match_dist;
    byte best_match;

    /* Iterate though 2**(3*ICLUT_BITS) colors, and set up the appropriate
    ** ICLUT entry based on a least-squares match against the opaque colors
    ** in our palette.
    **
    ** We build the ICLUT from the pre-gamma-correction palette, because
    ** we want to base our ICLUT entries on the uncorrected color values.
    ** TODO - Should we fill out the ICLUT with gamma-corrected values? */
    for (r = 0; r < 1<<ICLUT_BITS; r++)
    {
        for (g = 0; g < 1<<ICLUT_BITS; g++)
        {
            for (b = 0; b < 1<<ICLUT_BITS; b++)
            {
                best_match = 0;
                best_match_dist = INT_MAX;
                for (i = 0; i < OPAQUE_PALETTE_ENTRIES; i++)
                {
                    r_dist = r*ICLUT_SCALE - palette[i*4 + 0];
                    g_dist = g*ICLUT_SCALE - palette[i*4 + 1];
                    b_dist = b*ICLUT_SCALE - palette[i*4 + 2];
                    dist = r_dist*r_dist + g_dist*g_dist + b_dist*b_dist;
                    if (dist < best_match_dist)
                    {
                        best_match = i;
                        best_match_dist = dist;
                    }
                }
                gInverseCLUT[(r << 2*ICLUT_BITS) | (g << 1*ICLUT_BITS) | b] =
                    best_match;
            }
        }
    }
}

static void UpdateDirtyRect(overlay_t *overlay)
{
    int x, y, dw, dh;
    byte *src, *dst;

    /* If our dirty rectangle is empty, we don't need to do anything. */
    if (overlay->dl == overlay->dr || overlay->dt == overlay->db)
        return;

    if (overlay->format == Q2_FORMAT_BGR)
        src = overlay->raw_data + overlay->dt*overlay->stride + overlay->dl*3;
    else
        src = overlay->raw_data + overlay->dt*overlay->stride + overlay->dl*4;
    dst = overlay->indexed_data + overlay->dt*overlay->width + overlay->dl;
    dw = overlay->dr - overlay->dl;
    dh = overlay->db - overlay->dt;
    for (y = 0; y < dh; y++)
    {
        // TODO - Optimize this loop.
        switch (overlay->format) {
            case Q2_FORMAT_BGR:
                for (x = 0; x < dw; x++)
                    dst[x] = ICLUT(src[3*x+2], src[3*x+1], src[3*x+0]);
                break;

            case Q2_FORMAT_BGRA_PREMUL:
                for (x = 0; x < dw; x++)
                    if (src[4*x+3] == 255)
                        dst[x] = ICLUT(src[4*x+2], src[4*x+1], src[4*x+0]);
					else
						dst[x] = TRANSPARENT_COLOR;
                break;

            //default:
                // TODO - What should we do for unknown formats?
        }
                
        src += overlay->stride;
        dst += overlay->width;
    }
    overlay->dl = overlay->dt = overlay->db = overlay->dr = 0;
}

void R_DrawOverlays()
{
    size_t i, x, y;
    for (i = 0; i < gOverlayCount; i++)
    {
        overlay_t *ov;
        byte *dst, *src, b;
        
        ov = gOverlays[i];

        // If we're not shown, don't display anything.
        if (!ov->is_shown)
            continue;

        // If we're not fully on screen, don't display anything.
        if (ov->left < 0 || ov->top < 0 ||
            ov->left + ov->width > vid.width ||
            ov->top + ov->height > vid.height)
            continue;

        UpdateDirtyRect(ov);

        // This code is based very closely on Draw_Pic in r_draw.c.
        dst = vid.buffer + ov->top * vid.rowbytes + ov->left;
        src = ov->indexed_data;
        for (y = 0; y < ov->height; y++)
        {
            // TODO - This could be faster.
            // TODO - Should we take advantage of overlays known to be
            // lacking transparency?
            for (x = 0; x < ov->width; x++)
                if ((b=src[x]) != TRANSPARENT_COLOR)
                    dst[x] = b;
            dst += vid.rowbytes;
            src += ov->width;
        }
    }
}

overlay_t *R_OverlayNew(int format, byte *data,
                        size_t left, size_t top,
                        size_t width, size_t height,
                        int stride)
{
	overlay_t *ov;

    if (gOverlayCount == MAX_OVERLAYS)
    {
        ri.Sys_Error(ERR_FATAL, "Too many overlays.");
        return NULL;
    }

    ov = (overlay_t *) malloc(sizeof(overlay_t));

    ov->format       = format;
    ov->raw_data     = data;
    ov->left         = left;
    ov->top          = top;
    ov->width        = width;
    ov->height       = height;
    ov->stride       = stride;
    ov->is_shown     = 1;

    ov->dl           = 0;
    ov->dt           = 0;
    ov->dr           = width;
    ov->db           = height;
    ov->indexed_data = (byte *) malloc(width * height);
 
    gOverlays[gOverlayCount++] = ov;
	return ov;
}

void R_OverlayShow(overlay_t *overlay, int show)
{
    overlay->is_shown = show;
}

void R_OverlayMove(overlay_t *overlay, size_t left, size_t top)
{
    overlay->left = left;
    overlay->top  = top;
}

void R_OverlayDirtyRect(overlay_t *overlay, size_t left, size_t top,
                        size_t width, size_t height)
{
    int right, bottom;

    /* Convert from our unambiguous left/top/width/height to a LTRB with
    ** RB exclusive (which we don't use in the public API, because everybody
    ** assumes that RB are inclusive). */
    right = left + width;
    bottom = top + height;

    if (overlay->dl == overlay->dr ||
        overlay->dt == overlay->db)
    {
        /* The dirty rect is empty, so we'll just set it. */
        overlay->dl = left;
        overlay->dt = top;
        overlay->dr = right;
        overlay->db = bottom;
    }
    else
    {
        /* We need to extend the existing dirty rectangle. */
        if (left < overlay->dl)
            overlay->dl = left;
        if (top < overlay->dt)
            overlay->dt = top;
        if (right > overlay->dr)
            overlay->dr = right;
        if (bottom > overlay->db)
            overlay->db = bottom;
    }

    /* Clip our dirty rect to our bounding box. */
    if (overlay->dl < 0)
        overlay->dl = 0;
    if (overlay->dt < 0)
        overlay->dt = 0;
    if (overlay->dr > overlay->width)
        overlay->dr = overlay->width;
    if (overlay->db > overlay->height)
        overlay->db = overlay->height;
}

void R_OverlayDelete(overlay_t *overlay)
{
    size_t i;
    
    /* Delete the overlay from our list. */
    for (i = 0; i < gOverlayCount; i++)
    {
        if (gOverlays[i] == overlay)
        {
            memmove(&gOverlays[i], &gOverlays[i+1],
                    sizeof(overlay_t *) * (gOverlayCount - (i+1)));
            break;
        }
    }
    --gOverlayCount;

    /* Free the memory we allocated. */
    free(overlay->indexed_data);
    free(overlay);
}

#endif // IML_Q2_EXTENSIONS
