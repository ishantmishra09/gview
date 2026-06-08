/*
 * app.h - Central application state
 *
 * Defines the App struct that is passed through the entire program.
 * Every subsytem ( viewer, gallery, events ) reads from or writes to this.
 */

#pragma once

#include <SDL2/SDL.h>
#include <limits.h>
#include <stdbool.h>

#include "image.h"
#include "viewer.h"

#define MAX_GALLERY_FILES 1024

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;

  SDL_Cursor *cursor_arrow;
  SDL_Cursor *cursor_grab;

  Image *img; /* currently displayed image, NULL if none */
  ViewState view;

  bool running;
  bool panning;

  /* Anchor points recorded on mouse-down, used to compute drag delta */
  int pan_start_y;
  int pan_start_x;

  float pan_orig_y;
  float pan_orig_x;

  /* Sorted list of image paths in the same directory as the open file */
  char *gallery[MAX_GALLERY_FILES];
  int gallery_count;
  int current_index;
} App;

bool app_init(App *app, int argc, char **argv);
void app_shutdown(App *app);

bool app_load_image(App *app, const char *path);

void app_render(App *app);
