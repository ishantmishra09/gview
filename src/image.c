/*
 * image.c - Image loading via stb_image -> SDL Texture
 */

#include "image.h"

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

/*
 * Loads an image file into an SDL_Texture
 *
 * stb_image decodes the file and returns a raw pixel buffer.
 * We force 4 channels ( RGBA ) so the texture format is always consistent.
 * The pixel buffer is freed once the data is uploaded to the GPU.
 *
 * Returns NULL on failure (decode error or out of memory)
 */
Image *image_load(SDL_Renderer *renderer, const char *path) {

  int w, h, channels;

  unsigned char *pixels =
      stbi_load(path, &w, &h, &channels, 4); /* force RGBA */
  if (!pixels) {

    fprintf(stderr, "image_load: failed to load'%s': %s\n", path,
            stbi_failure_reason());
    return NULL;
  }

  /*
   * Downscale images that exceeds 4096px on either dimension.
   *
   * Uploading a 6000x4000 texture jus to display it at 800x600 wastes VRAM and
   * causes quality loss because the GPU skips too many pixels when downscaling
   * aggressively. Resizing on the CPU first with stbir produces a better result
   * since it samples all pixels properly.
   *
   * Tradeoff: zooming in beyond 4096px will show the resized version, not the
   * original pixels.
   */
  const int max_dim = 4096;
  unsigned char *resized = NULL;

  if (w > max_dim || h > max_dim) {

    /* Preserve aspect ratio: scale so the longer side becomes max_dim */
    float ratio = (w > h) ? (float)max_dim / w : (float)max_dim / h;
    int new_w = (int)(w * ratio);
    int new_h = (int)(h * ratio);

    resized = malloc(new_w * new_h * 4); /* 4 bytes per pixell (RGBA) */
    if (!resized) {

      stbi_image_free(pixels);
      return NULL;
    }

    stbir_resize_uint8_linear(pixels, w, h, 0, resized, new_w, new_h, 0,
                              STBIR_RGBA);

    stbi_image_free(pixels);
    pixels = resized;
    w = new_w;
    h = new_h;
  }

  SDL_Texture *tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                       SDL_TEXTUREACCESS_STATIC, w, h);
  if (!tex) {

    fprintf(stderr, "image_load: SDL_CreateTexture error: %s\n",
            SDL_GetError());
    if (resized)
      free(resized);
    else
      stbi_image_free(pixels);
    return NULL;
  }

  SDL_UpdateTexture(tex, NULL, pixels,
                    w * 4); /* pitch = width * 4 bytes ( RGBA ) */
  SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND); /* needed for PNG alpha */

  if (resized)
    free(resized);
  else
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
