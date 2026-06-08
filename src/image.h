/*
 * image.h - Image loading and lifetime management
 *
 * Wraps an SDL_Texture together with the original pixel dimensions so thhe
 * rest of the code doesn't need to query SDL every time.
 */

#pragma once

#include <SDL2/SDL.h>

typedef struct {
  SDL_Texture *texture;
  int width;
  int height;
} Image;

/* Loads an image from disk via stb_image and uploads it to a GPU Texture.
 * Returns NULL on failure. Caller must free with image_free(); */
Image *image_load(SDL_Renderer *renderer, const char *path);

/* Destroys the GPU texture and free the Image struct. Safe to call with NULL.
 */
void image_free(Image *img);
