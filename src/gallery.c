#define _GNU_SOURCE
#include "gallery.h"
#include "src/app.h"

#include <dirent.h>
#include <stdlib.h>
#include <string.h>

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

void gallery_clear(App *app) {

  for (int i = 0; i < app->gallery_count; i++)
    free(app->gallery[i]);

  app->gallery_count = 0;
  app->current_index = -1;
}

static int cmp_string(const void *a, const void *b) {

  return strcmp(*(char **)a, *(char **)b);
}

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

  for (int i = 0; i < app->gallery_count; i++) {

    if (!strcmp(app->gallery[i], current_file)) {

      app->current_index = i;
      break;
    }
  }

  return app->gallery_count > 0;
}

bool gallery_next(App *app) {

  if (app->gallery_count <= 1)
    return false;

  app->current_index++;

  if (app->current_index >= app->gallery_count)
    app->current_index = 0;

  return app_load_image(app, app->gallery[app->current_index]);
}

bool gallery_prev(App *app) {

  if (app->gallery_count <= 1)
    return false;

  app->current_index--;

  if (app->current_index < 0)
    app->current_index = app->gallery_count - 1;

  return app_load_image(app, app->gallery[app->current_index]);
}
