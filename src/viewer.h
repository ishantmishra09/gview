#pragma once

#include "image.h"
#include <SDL2/SDL.h>

typedef struct {
  float scale;
  float offset_x;
  float offset_y;
  int win_w;
  int win_h;
} ViewState;

void view_fit(ViewState *vs, const Image *img);

void view_zoom(ViewState *vs, float factor, int pivot_x, int pivot_y);

void view_render(SDL_Renderer *renderer, const Image *img, const ViewState *vs);
