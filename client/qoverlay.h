#ifndef Q2_OVERLAY_H
#define Q2_OVERLAY_H 1

#ifdef IML_Q2_EXTENSIONS

/* These are also declared in ref.h. */
struct overlay_s;
typedef struct overlay_s overlay_t;

/* Standard 24-bit RGB color, in reverse order. */
#define Q2_FORMAT_BGR         (1)

/* 32-bit graphics, with the color channels premultiplied and alpha 255
** opaque. */ 
#define Q2_FORMAT_BGRA_PREMUL (2)

/* Create a new overlay.  Overlays are drawn over the regular Quake 2
** graphics, but below the console.
**
**   format  - The pixel format of the input data.
**   data    - A pointer to co-ordinate 0,0 in the data.  The data
**             should be owned by somebody else, and must last until
**             the call to q2_overlay_delete.
**   left, top - Where to put the overlay on the screen.
**   width   - The width of the overlay, in pixels.
**   height  - The height of the overlay, in pixels.
**   stride  - The number of bytes between the start of row N and row N+1.
**             Negative values are allowed.
*/
extern overlay_t *q2_overlay_new(int format, unsigned char *data,
                                 size_t left, size_t top,
                                 size_t width, size_t height,
                                 int stride);

/* Move the overlay to a new position on screen.
**
**   overlay - The overlay to move.
**   left, top - The new position for the overlay.
*/
extern void q2_overlay_move(overlay_t *overlay, size_t left, size_t top);

/* Notify Quake 2 that the data in the overlay has changed.  This allows
** Quake 2 to update any cached format conversions.
**
**   overlay - The overlay to update.
**   left, top, width, height - The rectangle which has changed.  The
**             coordinates are relative to the overlay itself.
*/
extern void q2_overlay_dirty_rect(overlay_t *overlay, size_t left, size_t top,
                                  size_t width, size_t height);

/* Destroy the overlay. */
extern void q2_overlay_delete(overlay_t *overlay);

#endif /* IML_Q2_EXTENSIONS */
#endif /* Q2_OVERLAY_H */
