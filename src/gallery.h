#pragma once

#include "app.h"

bool gallery_build(App *app, const char *current_file);

bool gallery_next(App *app);

bool gallery_prev(App *app);

void gallery_clear(App *app);
