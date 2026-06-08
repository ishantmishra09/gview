/*
 * viewer.h - Zoom and pan state for the displayed image
 *
 * ViewState holds all the data needed to position and scale and image inside
 * the window. No SDL or image decoding happens here - it is pure geometry.
 *
 * Coordinate system:
 *  offset_x / offset_y are the top-left corner of the displayed image in window
 *  pixels. They can be negative when the image is larger the window and has
 *  been panned.
 */

#pragma once

#include "image.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
  float scale; /* current zoom level (1.0 = 1 image pixel : 1 screen pixel) */
  float fit_scale; /* scale when the image was last fit to the window; used to
                    detect whether the user has zoomed in */
  float offset_x;  /* top-left x of the image in window coordinates */
  float offset_y;  /* top-left y of the image in window coordinates */
  int win_w;
  int win_h;
} ViewState;

/* Scales the image to fit entirely within the window and centres it. */
void view_fit(ViewState *vs, const Image *img);

/*
 * Multiplies the current scale by `factor`, clamped to [ZOOM_MIN, ZOOM_MAX].
 * The zoom is anchored at ( pivot_x, pivot_y ) in window coordinates so that
 * the pixel under the cursor stays in place.
 */
void view_zoom(ViewState *vs, const Image *img, float factor, int pivot_x,
               int pivot_y);

/* Returns true if the user has zoomed in beyound the fit-to-window scale. */
bool view_is_zoomed(const ViewState *vs);

/*
 * Clamps offset_x/ offset_y so the image cannot be panned beyond the window
 * edges. When the image is smaller than the window it is re-centered instead.
 */
void view_clamp(ViewState *vs, const Image *img);

/* Renders the image to the SDL renderer using the current ViewState. */
void view_render(SDL_Renderer *renderer, const Image *img, const ViewState *vs);
