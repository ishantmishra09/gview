#include <gtk/gtk.h>

#define WINDOW_TITLE "gview"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static void activate(GtkApplication *app, gpointer user_data) {
  (void)user_data;

  GtkWidget *window;

  g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme",
               TRUE, NULL);

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), WINDOW_TITLE);
  gtk_window_set_default_size(GTK_WINDOW(window), WINDOW_WIDTH, WINDOW_HEIGHT);

  gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("dev.gview.app", G_APPLICATION_DEFAULT_FLAGS);

  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

  status = g_application_run(G_APPLICATION(app), argc, argv);

  g_object_unref(app);

  return status;
}
