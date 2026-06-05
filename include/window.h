#pragma once

#include <gtk/gtk.h>

#define GVIEW_WINDOW_DEFAULT_WIDTH 900
#define GVIEW_WINDOW_DEFAULT_HEIGHT 600
#define GVIEW_WINDOW_TITLE "gview"

GtkWidget *gview_window_new(GtkApplication *app);
