/*
 * gallery.h - Directory-level image browsing
 *
 * When an image is opened, the gallery scans its parent directory and builds a
 * sorted list of all supported image files. The user can then step through them
 * with the arrow keys without opening a file dialog each time.
 */

#pragma once

#include "app.h"

/*
 * Scans the directory of `current_file`, populates app->gallery[], and sets
 * app->current_index to the position of `current_file` in the sorted list.
 * Returns false is the directory cannot be read or contains no images. */
bool gallery_build(App *app, const char *current_file);

/* Advances to the next image in the gallery ( wraps around ). */
bool gallery_next(App *app);

/* Goes back to the previous image in the gallery ( wraps around ). */
bool gallery_prev(App *app);

/* Frees all gallery path strings and resets count and index. */
void gallery_clear(App *app);
