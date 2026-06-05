#include "viewer.h"
#include <math.h>

void view_fit(ViewState *vs, const Image *img) {

  float scale_x = (float)vs->win_w / (float)img->width;
  float scale_y = (float)vs->win_h / (float)img->height;
  vs->scale = fminf(scale_x, scale_y);

  float disp_w = img->width * vs->scale;
  float disp_h = img->height * vs->scale;
  vs->offset_x = (vs->win_w - disp_w) / 2.0f;
  vs->offset_y = (vs->win_h - disp_h) / 2.0f;
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
