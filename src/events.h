/*
 * events.h - SDL event dispatcher
 */

#pragma once

#include "app.h"

/* Hanles a single SDL_Event. Called once per event in the main loop. */
void handle_event(App *app, SDL_Event *event);
