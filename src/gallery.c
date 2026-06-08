/*
 * gallery.c - Directory scanning and sequential image navigation
 */

#define _GNU_SOURCE
#include "gallery.h"
#include "src/app.h"

#include <dirent.h>
#include <stdlib.h>
#include <string.h>

/* Returns true if the filename's extension is a supported image format. */
static bool is_image_file(const char *name) {

  const char *ext = strrchr(name, '.');

  if (!ext)
    return false;

  ext++;

  return !strcasecmp(ext, "jpg") || !strcasecmp(ext, "jpeg") ||
         !strcasecmp(ext, "png") || !strcasecmp(ext, "bmp") ||
         !strcasecmp(ext, "webp") || !strcasecmp(ext, "tif") ||
         !strcasecmp(ext, "ppm") || !strcasecmp(ext, "tiff");
}

/* Frees all heap-allocated path strings and resets the gallery to empty. */
void gallery_clear(App *app) {

  for (int i = 0; i < app->gallery_count; i++)
    free(app->gallery[i]);

  app->gallery_count = 0;
  app->current_index = -1;
}

static int cmp_string(const void *a, const void *b) {

  return strcmp(*(char **)a, *(char **)b);
}

/*
 * Builds the gallery from the directory that contains `current_file`.
 *
 * Steps:
 *  1. Strip the filename to get the directory path.
 *  2. Read every entry - keep only those with supported image extensions.
 *  3. Sort alphabetically so arrow-key navigation feels natural.
 *  4. Find `current_file` in the sorted list and store its index.
 */
bool gallery_build(App *app, const char *current_file) {

  gallery_clear(app);

  char dir[PATH_MAX];

  strcpy(dir, current_file);

  char *slash = strrchr(dir, '/');

#ifdef _WIN32
  if (!slash)
    slash = strrchr(dir, '\\');
#endif

  if (!slash)
    return false;

  *slash = '\0';

  DIR *d = opendir(dir);
  if (!d)
    return false;

  struct dirent *entry;

  while ((entry = readdir(d))) {

    if (!is_image_file(entry->d_name))
      continue;

    char full[PATH_MAX];

    int written = snprintf(full, sizeof(full), "%s/%s", dir, entry->d_name);
    if (written < 0 || written >= (int)sizeof(full))
      continue;

    app->gallery[app->gallery_count++] = strdup(full);

    if (app->gallery_count >= MAX_GALLERY_FILES)
      break;
  }

  closedir(d);

  qsort(app->gallery, app->gallery_count, sizeof(char *), cmp_string);

  /* Locate the currently open file so next/prev work from the right position */
  for (int i = 0; i < app->gallery_count; i++) {

    if (!strcmp(app->gallery[i], current_file)) {

      app->current_index = i;
      break;
    }
  }

  return app->gallery_count > 0;
}

/* Loads the next image in the sorted list, wrapping from last to first. */
bool gallery_next(App *app) {

  if (app->gallery_count <= 1)
    return false;

  app->current_index++;

  if (app->current_index >= app->gallery_count)
    app->current_index = 0;

  return app_load_image(app, app->gallery[app->current_index]);
}

/* Loads the previous image in the sorted list, wrapping from last to first. */
bool gallery_prev(App *app) {

  if (app->gallery_count <= 1)
    return false;

  app->current_index--;

  if (app->current_index < 0)
    app->current_index = app->gallery_count - 1;

  return app_load_image(app, app->gallery[app->current_index]);
}
