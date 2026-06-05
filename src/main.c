#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#include "image.h"
#include "viewer.h"

#define WINDOW_TITLE "gview"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char *argv[]) {

  if (argc < 2) {

    fprintf(stderr, "Usage: gview <image_path>\n");
    return 1;
  }

  (void)argv;

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {

    fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow(
      WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (!window) {

    fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer) {

    fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  Image *img = image_load(renderer, argv[1]);
  if (!img) {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  /* Set window title to filename */
  char title[512];
  snprintf(title, sizeof(title), "gview - %s", argv[1]);
  SDL_SetWindowTitle(window, title);

  ViewState vs = {.win_w = WINDOW_WIDTH, .win_h = WINDOW_HEIGHT};
  view_fit(&vs, img);

  bool running = true;
  SDL_Event event;

  while (running) {

    while (SDL_PollEvent(&event)) {

      switch (event.type) {

      case SDL_QUIT:
        running = false;
        break;

      case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {

          vs.win_w = event.window.data1;
          vs.win_h = event.window.data2;
          view_fit(&vs, img);
        }
        break;

      case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
          running = false;
        break;
      }
    }

    SDL_SetRenderDrawColor(renderer, 30, 30, 30,
                           255); /* dark grey background */
    SDL_RenderClear(renderer);

    view_render(renderer, img, &vs);
    SDL_RenderPresent(renderer);
  }

  image_free(img);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
