#include "viewer.h"
#include <math.h>

#define ZOOM_MIN 0.05f
#define ZOOM_MAX 23.0f
#define ZOOM_FIT_EPSILON 0.001f

void view_fit(ViewState *vs, const Image *img) {

  float scale_x = (float)vs->win_w / (float)img->width;
  float scale_y = (float)vs->win_h / (float)img->height;
  vs->scale = fminf(scale_x, scale_y);
  vs->fit_scale = vs->scale;

  float disp_w = img->width * vs->scale;
  float disp_h = img->height * vs->scale;
  vs->offset_x = (vs->win_w - disp_w) / 2.0f;
  vs->offset_y = (vs->win_h - disp_h) / 2.0f;
}

void view_zoom(ViewState *vs, const Image *img, float factor, int pivot_x,
               int pivot_y) {

  float new_scale = vs->scale * factor;
  if (new_scale < ZOOM_MIN)
    new_scale = ZOOM_MIN;
  if (new_scale > ZOOM_MAX)
    new_scale = ZOOM_MAX;

  float ratio = new_scale / vs->scale;
  vs->offset_x = pivot_x - ratio * (pivot_x - vs->offset_x);
  vs->offset_y = pivot_y - ratio * (pivot_y - vs->offset_y);
  vs->scale = new_scale;

  view_clamp(vs, img);
}

bool view_is_zoomed(const ViewState *vs) {

  return vs->scale > vs->fit_scale + ZOOM_FIT_EPSILON;
}

void view_clamp(ViewState *vs, const Image *img) {

  float disp_w = img->width * vs->scale;
  float disp_h = img->height * vs->scale;

  /* Horizontal */
  if (disp_w <= vs->win_w) {

    vs->offset_x = (vs->win_w - disp_w) / 2.0f;
  } else {

    float min_x = vs->win_w - disp_w;
    if (vs->offset_x < min_x)
      vs->offset_x = min_x;
    if (vs->offset_x > 0)
      vs->offset_x = 0;
  }

  /* Vertical */
  if (disp_h <= vs->win_h) {

    vs->offset_y = (vs->win_h - disp_h) / 2.0f;
  } else {

    float min_y = vs->win_h - disp_h;
    if (vs->offset_y < min_y)
      vs->offset_y = min_y;
    if (vs->offset_y > 0)
      vs->offset_y = 0;
  }
}

void view_render(SDL_Renderer *renderer, const Image *img,
                 const ViewState *vs) {

  SDL_Rect dst = (SDL_Rect){
      .x = (int)vs->offset_x,
      .y = (int)vs->offset_y,
      .w = (int)(img->width * vs->scale),
      .h = (int)(img->height * vs->scale),
  };

  SDL_RenderCopy(renderer, img->texture, NULL, &dst);
}
