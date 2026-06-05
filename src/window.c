#include "window.h"

GtkWidget *gview_window_new(GtkApplication *app) {

  GtkWidget *window = gtk_application_window_new(app);

  gtk_window_set_title(GTK_WINDOW(window), GVIEW_WINDOW_TITLE);
  gtk_window_set_default_size(GTK_WINDOW(window), GVIEW_WINDOW_DEFAULT_WIDTH,
                              GVIEW_WINDOW_DEFAULT_HEIGHT);

  return window;
}
