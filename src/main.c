/*
 * main.c - Entry point
 *
 * Initialises the application, runs the main event/render loop, and performs
 * shutdown cleanup before exit.
 */

#include "app.h"
#include "events.h"

int main(int argc, char *argv[]) {

  App app = {0};

  if (!app_init(&app, argc, argv))
    return 1;

  SDL_Event event;

  while (app.running) {

    while (SDL_PollEvent(&event)) {

      handle_event(&app, &event);
    }

    app_render(&app);
  }

  app_shutdown(&app);

  return 0;
}
