#include "app.h"
#include "window.h"

static void on_activate(GtkApplication *app, gpointer user_data) {

  (void)user_data;

  GtkWidget *window = gview_window_new(app);
  gtk_window_present(GTK_WINDOW(window));
}

GtkApplication *gview_app_new(void) {

  GtkApplication *app =
      gtk_application_new(GVIEW_APP_ID, G_APPLICATION_FLAGS_NONE);

  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

  return app;
}
