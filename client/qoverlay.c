#ifdef IML_Q2_EXTENSIONS

#include "client.h"
#include "qoverlay.h"

#define MAX_OVERLAYS (128)

// We keep a list of all active overlays so we can draw them.
static size_t gOverlayCount = 0;
overlay_t *gOverlays[MAX_OVERLAYS];

overlay_t *q2_overlay_new(int format, unsigned char *data,
                          size_t left, size_t top,
                          size_t width, size_t height,
                          int stride)
{
	overlay_t *ov;

    if (gOverlayCount == MAX_OVERLAYS)
    {
        Sys_Error(ERR_FATAL, "Too many overlays.");
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
    ov->ref_data     = NULL;
 
    gOverlays[gOverlayCount++] = ov;

    if (re.OverlayInitializeRefData)
        re.OverlayInitializeRefData(ov);

    return ov;
}

void q2_overlay_show(overlay_t *overlay, int show)
{
    overlay->is_shown = show;
}

void q2_overlay_move(overlay_t *overlay, size_t left, size_t top)
{
    overlay->left = left;
    overlay->top  = top;
}

void q2_overlay_dirty_rect(overlay_t *overlay, size_t left, size_t top,
                           size_t width, size_t height)
{
    int right, bottom;

    // Convert from our unambiguous left/top/width/height to a LTRB with
    // RB exclusive (which we don't use in the public API, because everybody
    // assumes that RB are inclusive).
    right = left + width;
    bottom = top + height;

    if (overlay->dl == overlay->dr ||
        overlay->dt == overlay->db)
    {
        // The dirty rect is empty, so we'll just set it.
        overlay->dl = left;
        overlay->dt = top;
        overlay->dr = right;
        overlay->db = bottom;
    }
    else
    {
        // We need to extend the existing dirty rectangle.
        if (left < overlay->dl)
            overlay->dl = left;
        if (top < overlay->dt)
            overlay->dt = top;
        if (right > overlay->dr)
            overlay->dr = right;
        if (bottom > overlay->db)
            overlay->db = bottom;
    }

    // Clip our dirty rect to our bounding box.
    if (overlay->dl < 0)
        overlay->dl = 0;
    if (overlay->dt < 0)
        overlay->dt = 0;
    if (overlay->dr > overlay->width)
        overlay->dr = overlay->width;
    if (overlay->db > overlay->height)
        overlay->db = overlay->height;
}

void q2_overlay_delete(overlay_t *overlay)
{
    size_t i;
    
    // Delete the overlay from our list.
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

    // Free the memory we allocated.
    if (re.OverlayReleaseRefData)
        re.OverlayReleaseRefData(overlay);
    free(overlay);
}

static void UpdateDirtyRect(overlay_t *overlay)
{
    int x, y, dw, dh;
    byte *src, *dst;

    // If our dirty rectangle is empty, we don't need to do anything.
    if (overlay->dl == overlay->dr || overlay->dt == overlay->db)
        return;

    if (re.OverlayUpdateDirtyRect)
        re.OverlayUpdateDirtyRect(overlay);

    overlay->dl = overlay->dt = overlay->db = overlay->dr = 0;
}

void q2_draw_overlays()
{
    size_t i;
    for (i = 0; i < gOverlayCount; i++)
    {
        overlay_t *ov;
        byte *dst, *src, b;
        
        ov = gOverlays[i];

        // If we're not shown, don't display anything.
        if (!ov->is_shown)
            continue;

        // If we're not fully on screen, don't display anything.
		// BUG - This works around a limitation in our graphics driver.
        if (ov->left < 0 || ov->top < 0 ||
            ov->left + ov->width > viddef.width ||
            ov->top + ov->height > viddef.height)
            continue;

        UpdateDirtyRect(ov);

        if (re.OverlayDraw)
            re.OverlayDraw(ov);
    }
}

int q2_mouse_is_interactive()
{
    // The mouse is interactive only if (1) the cls data structure thinks
    // it should be and (2) key input is currently being directed to the
    // Quake 2 game (and not the console or menu or something like that).
    // The latter condition automatically turns off interactive mouse
    // whenever the user goes into some kind of special Quake 2 mode.
    return (cls.interactivemouse && cls.key_dest == key_game);
}

void q2_enable_gui(int enable)
{
    cls.disable_gui = !enable;
}

#endif /* IML_Q2_EXTENSIONS */
