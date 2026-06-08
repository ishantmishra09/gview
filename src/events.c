/*
 * events.c - Keyboard, mouse, and window event handling
 *
 * Keybindings:
 *  Q / ESCAPE      Quit
 *  O               Open file dialog
 *  F               Fit image to window
 *  Left / Right    Previous / next image in gallery
 *  + / - (or KP)   Zoom in / out ( centered on window )
 *  Mouse wheel     Zoom in / out
 *  Left drag       Pan ( only when zoomed in )
 */

#include "events.h"
#include "app.h"
#include "gallery.h"
#include "nfd_g.h"
#include "viewer.h"

#include <nfd.h>

#define ZOOM_STEP 1.1f /* 10% per zoom step */

void handle_event(App *app, SDL_Event *event) {

  switch (event->type) {

  case SDL_QUIT:
    app->running = false;
    break;

  case SDL_KEYDOWN: {
    switch (event->key.keysym.sym) {

    case SDLK_ESCAPE:
    case SDLK_q:
      app->running = false;
      break;

    case SDLK_o: {

      nfdchar_t *path = open_dialog();
      if (path) {

        app_load_image(app, path);
        gallery_build(app, path); /* rebuild gallery for the new directory */

        NFD_FreePathU8(path);
      }
    } break;

    case SDLK_f:
      if (app->img) {

        view_fit(&app->view, app->img);
      }
      break;

    case SDLK_RIGHT:
      if (app->img) {

        gallery_next(app);
      }
      break;

    case SDLK_LEFT:
      if (app->img) {

        gallery_prev(app);
      }
      break;

    case SDLK_PLUS:
    case SDLK_EQUALS:
    case SDLK_KP_PLUS:
      if (app->img) {

        view_zoom(&app->view, app->img, ZOOM_STEP, app->view.win_w / 2,
                  app->view.win_h / 2);
      }
      break;

    case SDLK_MINUS:
    case SDLK_KP_MINUS:
      if (app->img) {

        view_zoom(&app->view, app->img, 1.0f / ZOOM_STEP, app->view.win_w / 2,
                  app->view.win_h / 2);
      }
      break;
    }
  } break;

  case SDL_MOUSEWHEEL: {
    if (app->img) {
      /* Zoom towards the window centre; a mouse-position pivot could be added
       * later */
      if (event->wheel.y > 0)
        view_zoom(&app->view, app->img, ZOOM_STEP, app->view.win_w / 2,
                  app->view.win_h / 2);
      else if (event->wheel.y < 0)
        view_zoom(&app->view, app->img, 1.0f / ZOOM_STEP, app->view.win_w / 2,
                  app->view.win_h / 2);
    }
  } break;

  case SDL_MOUSEBUTTONDOWN:

    if (app->img && event->button.button == SDL_BUTTON_LEFT &&
        view_is_zoomed(&app->view)) {

      app->panning = true;
      app->pan_start_y = event->button.y;
      app->pan_orig_y = app->view.offset_y;
      app->pan_start_x = event->button.x;
      app->pan_orig_x = app->view.offset_x;

      SDL_SetCursor(app->cursor_grab);
    }
    break;

  case SDL_MOUSEBUTTONUP:
    if (app->img) {

      if (event->button.button == SDL_BUTTON_LEFT && app->panning) {

        app->panning = false;
        SDL_SetCursor(app->cursor_arrow);
      }
    }
    break;

  case SDL_MOUSEMOTION:
    if (app->panning && app->img) {

      /* Translate by the delta from where the drag started */
      app->view.offset_x =
          app->pan_orig_x + (event->motion.x - app->pan_start_x);
      app->view.offset_y =
          app->pan_orig_y + (event->motion.y - app->pan_start_y);

      view_clamp(&app->view, app->img);
    }
    break;

  case SDL_WINDOWEVENT:

    if (event->window.event == SDL_WINDOWEVENT_RESIZED) {

      int w = event->window.data1;
      int h = event->window.data2;

      app->view.win_w = w;
      app->view.win_h = h;

      if (app->img) {
        if (!view_is_zoomed(&app->view)) {

          view_fit(&app->view, app->img); /* re-fit when not zoomed in+ */
        } else {

          view_clamp(&app->view,
                     app->img); /* keep image in bounds when zoomed in */
        }
      }
    }
    break;
  }
}
