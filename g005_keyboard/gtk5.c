#include <gtk/gtk.h>
#include <cairo.h>

static gboolean on_key_press(GtkWidget *win, GdkEventKey *event, gpointer data) {
  gchar *s = event->string;
  guint q = event->keyval / 256;
  guint k = event->keyval % 256;

  if (q == 255 && (k == 8 || k == 13 || k == 27)) s = "...";

  printf("Key %d = %d | %d ; %d '%s'\n",
    event->keyval,
    event->keyval / 256,
    event->keyval % 256,
    event->hardware_keycode,
    s);

  return TRUE;
}

static void on_activate(GtkApplication *app) {
  GtkWidget *win = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(win), "GTK Test 5");

  g_signal_connect(win, "key-press-event", G_CALLBACK(on_key_press), NULL);

  gtk_widget_show_all(win);
}

int main(int argc, char *argv[]) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("com.example.GtkApplication", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}

// Also see https://developer.gnome.org/gdk3/stable/gdk3-Event-Structures.html#GdkEventKey
