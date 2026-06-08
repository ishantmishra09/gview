#pragma once

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <limits.h>

#include "image.h"
#include "viewer.h"

#define MAX_GALLERY_FILES 4096

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;

  SDL_Cursor *cursor_arrow;
  SDL_Cursor *cursor_grab;

  Image *img;
  ViewState view;

  bool running;
  bool panning;

  int pan_start_y;
  float pan_orig_y;
  int pan_start_x;
  float pan_orig_x;

  char *gallery[MAX_GALLERY_FILES];
  int gallery_count;
  int current_index;
} App;

bool app_init(App *app, int argc, char **argv);
void app_shutdown(App *app);

bool app_load_image(App *app, const char *path);

void app_render(App *app);
