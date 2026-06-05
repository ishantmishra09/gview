#pragma once

#include <SDL2/SDL.h>

typedef struct {
  SDL_Texture *texture;
  int width;
  int height;
} Image;

Image *image_load(SDL_Renderer *renderer, const char *path);

void image_free(Image *img);
