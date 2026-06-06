#pragma once

#include "image.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
  float scale;
  float fit_scale;
  float offset_x;
  float offset_y;
  int win_w;
  int win_h;
} ViewState;

void view_fit(ViewState *vs, const Image *img);

void view_zoom(ViewState *vs, const Image *img, float factor, int pivot_x,
               int pivot_y);

bool view_is_zoomed(const ViewState *vs);

void view_clamp(ViewState *vs, const Image *img);

void view_render(SDL_Renderer *renderer, const Image *img, const ViewState *vs);
