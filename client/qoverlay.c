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

void q2_overlay_show(overlay_t *overlay, int show)
{
    if (re.OverlayShow)
        re.OverlayShow(overlay, show);
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
