#include "viewer.h"
#include <math.h>

#define ZOOM_MIN 0.05f
#define ZOOM_MAX 23.0f

void view_fit(ViewState *vs, const Image *img) {

  float scale_x = (float)vs->win_w / (float)img->width;
  float scale_y = (float)vs->win_h / (float)img->height;
  vs->scale = fminf(scale_x, scale_y);

  float disp_w = img->width * vs->scale;
  float disp_h = img->height * vs->scale;
  vs->offset_x = (vs->win_w - disp_w) / 2.0f;
  vs->offset_y = (vs->win_h - disp_h) / 2.0f;
}

void view_zoom(ViewState *vs, float factor, int pivot_x, int pivot_y) {

  float new_scale = vs->scale * factor;
  if (new_scale < ZOOM_MIN)
    new_scale = ZOOM_MIN;
  if (new_scale > ZOOM_MAX)
    new_scale = ZOOM_MAX;

  float ratio = new_scale / vs->scale;
  vs->offset_x = pivot_x - ratio * (pivot_x - vs->offset_x);
  vs->offset_y = pivot_y - ratio * (pivot_y - vs->offset_y);
  vs->scale = new_scale;
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
