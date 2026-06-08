/*
 * app.c - Application lifecycle ( init, render, shutdown ) and image loading
 */

#include "app.h"
#include "gallery.h"
#include "nfd_g.h"

#define WINDOW_TITLE "gview"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_MINIMUM_WIDTH 400
#define WINDOW_MINIMUM_HEIGHT 300

static void set_window_title(App *app, const char *path) {

  const char *filename = strrchr(path, '/');

#ifdef _WIN32
  if (!filename)
    filename = strrchr(path, '\\');
#endif

  filename = filename ? filename + 1 : path;

  char title[512];
  snprintf(title, sizeof(title), "gview - %s", filename);
  SDL_SetWindowTitle(app->window, title);
}

/*
 * Loads an image from disk, replaces the current one, resets the view to
 * fit-to-window, and updated title bar.
 * Returns false is the image could not be loaded ( current image is kept )
 */
bool app_load_image(App *app, const char *path) {

  Image *next = image_load(app->renderer, path);

  if (!next)
    return false;

  image_free(app->img);

  app->img = next;

  view_fit(&app->view, app->img);

  set_window_title(app, path);

  return true;
}

bool app_init(App *app, int argc, char **argv) {

  (void)argc;
  (void)argv;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {

    fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
    return false;
  }

  if (NFD_Init() != NFD_OKAY) {

    fprintf(stderr, "NFD_Init error: %s\n", NFD_GetError());
    return false;
  }

  app->window = SDL_CreateWindow(
      WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (!app->window) {

    fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
    return false;
  }

  SDL_SetWindowMinimumSize(app->window, WINDOW_MINIMUM_WIDTH,
                           WINDOW_MINIMUM_HEIGHT);

  app->renderer = SDL_CreateRenderer(
      app->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!app->renderer) {

    fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError());
    return false;
  }

  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); /* 1 ( bilinear -> smooth) */

  app->view.win_w = WINDOW_WIDTH;
  app->view.win_h = WINDOW_HEIGHT;

  app->gallery_count = 0;
  app->current_index = -1;

  app->cursor_arrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
  app->cursor_grab = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);

  app->running = true;

  app->img = NULL;

  return true;
}

void app_render(App *app) {

  SDL_SetRenderDrawColor(app->renderer, 30, 30, 30,
                         255); /* dark grey background */
  SDL_RenderClear(app->renderer);

  if (app->img) {

    view_render(app->renderer, app->img, &app->view);
  }

  SDL_RenderPresent(app->renderer);
}

void app_shutdown(App *app) {

  SDL_FreeCursor(app->cursor_arrow);
  SDL_FreeCursor(app->cursor_grab);

  image_free(app->img);

  gallery_clear(app);

  SDL_DestroyRenderer(app->renderer);
  SDL_DestroyWindow(app->window);

  NFD_Quit();
  SDL_Quit();
}
