#ifdef IML_Q2_EXTENSIONS

#include "client.h"
#include "qoverlay.h"

overlay_t *q2_overlay_new(int format, unsigned char *data,
                          size_t left, size_t top,
                          size_t width, size_t height,
                          int stride)
{
    if (re.OverlayNew)
        return re.OverlayNew(format, data, left, top, width, height, stride);
    else
        return NULL;
}

void q2_overlay_move(overlay_t *overlay, size_t left, size_t top)
{
    if (re.OverlayMove)
        re.OverlayMove(overlay, left, top);
}

void q2_overlay_dirty_rect(overlay_t *overlay, size_t left, size_t top,
                           size_t width, size_t height)
{
    if (re.OverlayDirtyRect)
        re.OverlayDirtyRect(overlay, left, top, width, height);
}

void q2_overlay_delete(overlay_t *overlay)
{
    if (re.OverlayDelete)
        re.OverlayDelete(overlay);
}

#endif /* IML_Q2_EXTENSIONS */
