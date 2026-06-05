#include "image.h"

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Image *image_load(SDL_Renderer *renderer, const char *path) {

  int w, h, channels;

  unsigned char *pixels =
      stbi_load(path, &w, &h, &channels, 4); /* force RGBA */
  if (!pixels) {

    fprintf(stderr, "image_load: failed to load'%s': %s\n", path,
            stbi_failure_reason());
    return NULL;
  }

  SDL_Texture *tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                       SDL_TEXTUREACCESS_STATIC, w, h);
  if (!tex) {

    fprintf(stderr, "image_load: SDL_CreateTexture error: %s\n",
            SDL_GetError());
    stbi_image_free(pixels);
    return NULL;
  }

  SDL_UpdateTexture(tex, NULL, pixels, w * 4);
  SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
  stbi_image_free(pixels);

  Image *img = (Image *)malloc(sizeof(Image));
  if (!img) {

    SDL_DestroyTexture(tex);
    return NULL;
  }

  img->texture = tex;
  img->width = w;
  img->height = h;
  return img;
}

void image_free(Image *img) {

  if (!img)
    return;
  SDL_DestroyTexture(img->texture);
  free(img);
}
