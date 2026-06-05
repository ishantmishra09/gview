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

void view_render(SDL_Renderer *renderer, const Image *img, const ViewState *vs);
