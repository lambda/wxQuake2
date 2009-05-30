#ifdef IML_Q2_EXTENSIONS

#include <limits.h>
#include "r_local.h"

#define ICLUT_BITS   (5)
#define IGNORED_BITS (8 - ICLUT_BITS)
#define OPAQUE_PALETTE_ENTRIES (255) /* 256 is transparent */

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

void R_OverlayUpdateDirtyRect(overlay_t *overlay)
{
    int x, y, dw, dh;
    byte *indexed_data, *src, *dst;

    indexed_data = (byte *) overlay->ref_data;
    if (overlay->format == Q2_FORMAT_BGR)
        src = overlay->raw_data + overlay->dt*overlay->stride + overlay->dl*3;
    else
        src = overlay->raw_data + overlay->dt*overlay->stride + overlay->dl*4;
    dst = indexed_data + overlay->dt*overlay->width + overlay->dl;
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

            case Q2_FORMAT_BGRX:
                for (x = 0; x < dw; x++)
                    dst[x] = ICLUT(src[4*x+2], src[4*x+1], src[4*x+0]);
                break;

            //default:
                // TODO - What should we do for unknown formats?
        }
                
        src += overlay->stride;
        dst += overlay->width;
    }
}

struct ovrect {
    int x0;
    int y0;
    int x1;
    int y1;
};

static int clamp_coord(int coord, int minimum, int maximum)
{
    if (coord < minimum)
        return minimum;
    else if (coord > maximum)
        return maximum;
    else
        return coord;
}

static void intersect_rect(struct ovrect *ra, struct ovrect *rb)
{
    ra->x0 = clamp_coord(ra->x0, rb->x0, rb->x1);
    ra->y0 = clamp_coord(ra->y0, rb->y0, rb->y1);
    ra->x1 = clamp_coord(ra->x1, rb->x0, rb->x1);
    ra->y1 = clamp_coord(ra->y1, rb->y0, rb->y1);
}

void R_OverlayDraw(overlay_t *overlay)
{
    struct ovrect overlay_rect;
    struct ovrect screen_rect;
    byte *indexed_data;
    size_t x, y;
    byte *dst, *src, *s, *d, b;

    // Figure out which part of the overlay we need to draw (in overlay
    // coordinates, not screen coordinates).
    overlay_rect.x0 = 0;
    overlay_rect.y0 = 0;
    overlay_rect.x1 = overlay->width;
    overlay_rect.y1 = overlay->height;
    screen_rect.x0  = 0          - overlay->left;
    screen_rect.y0  = 0          - overlay->top;
    screen_rect.x1  = vid.width  - overlay->left;
    screen_rect.y1  = vid.height - overlay->top;
    intersect_rect(&overlay_rect, &screen_rect);

    // Give up now if we're not supposed to draw anything.
    if (overlay_rect.x0>=overlay_rect.x1 || overlay_rect.y0>=overlay_rect.y1)
        return;

    // Set up some pointers into our source and destination buffers.
    indexed_data = (byte *) overlay->ref_data;
    dst = (vid.buffer +
           (overlay->top+overlay_rect.y0) * vid.rowbytes +
           (overlay->left+overlay_rect.x0));
    src = indexed_data + overlay_rect.y0*overlay->width + overlay_rect.x0;

    // This code is based very closely on Draw_Pic in r_draw.c, and on
    // the various blitters in wx/src/DrawingAreaOpt.cpp.
    for (y = overlay_rect.y1 - overlay_rect.y0; y > 0; y--)
    {
        // TODO - Should we take advantage of overlays known to be
        // lacking transparency?
        for (x = overlay_rect.x1 - overlay_rect.x0, s = src, d = dst;
             x > 0; x--, s++, d++)
            if ((b=*s) != TRANSPARENT_COLOR)
                *d = b;
        dst += vid.rowbytes;
        src += overlay->width;
    }
}

void R_OverlayInitializeRefData(overlay_t *overlay)
{
    // Allocate space for indexed_data.
    overlay->ref_data = (byte *) malloc(overlay->width * overlay->height);
}


void R_OverlayReleaseRefData(overlay_t *overlay)
{
    // Release space for index_data.
    free(overlay->ref_data);
}

#endif // IML_Q2_EXTENSIONS
