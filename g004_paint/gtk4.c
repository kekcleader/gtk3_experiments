#include <gtk/gtk.h>
#include <cairo.h>

cairo_surface_t *surface = NULL;

static void clear_surface(cairo_surface_t *sur) {
  cairo_t *cr = cairo_create(sur);

  cairo_set_source_rgb(cr, 0, 0.8, 0);
  cairo_paint(cr);

  cairo_destroy(cr);
}

static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
  cairo_set_source_surface(cr, surface, 0, 0);
  cairo_paint(cr);
  return TRUE;
}

static void on_destroy() {
  if (surface) {
    cairo_surface_destroy(surface);
  }
}

static void paint(GtkWidget *widget, int x, int y) {
  cairo_t *cr;
  cr = cairo_create(surface);

  cairo_set_source_rgb(cr, 0, 0.4, 1);
  cairo_rectangle(cr, x - 3, y - 3, 7, 7);
  cairo_fill(cr);

  cairo_destroy(cr);
  gtk_widget_queue_draw_area(widget, x - 3, y - 3, 7, 7);
}

static gboolean on_move(GtkWidget *win, GdkEventMotion *event, gpointer data) {
  if (event->state & GDK_BUTTON1_MASK) {
    paint(win, event->x, event->y);
  }
  return TRUE;
}

static gboolean on_click(GtkWidget *win, GdkEventButton *event, gpointer data) {
  if (event->button == GDK_BUTTON_PRIMARY) {
    gdouble x = event->x;
    gdouble y = event->y;
    paint(win, x, y);
  } else if (event->button == GDK_BUTTON_SECONDARY) {
    clear_surface(surface);
    gtk_widget_queue_draw(win);
  }
  return TRUE;
}

static gboolean on_configure(GtkWidget *win, GdkEventConfigure *event, gpointer data) {
  cairo_surface_t *new;
  gint w, h;

  gtk_window_get_size(GTK_WINDOW(win), &w, &h);
  new = gdk_window_create_similar_surface(
    gtk_widget_get_window(win), CAIRO_CONTENT_COLOR, w, h);
  clear_surface(new);

  if (surface) {
    cairo_t *cr = cairo_create(new);
    cairo_set_source_surface(cr, surface, 0, 0);
    cairo_paint(cr);
    cairo_destroy(cr);
    
    cairo_surface_destroy(surface);
  }

  surface = new;

  gdk_window_set_event_compression(gtk_widget_get_window(win), FALSE);
  return FALSE;
}

static void on_activate(GtkApplication *app) {
  GtkWidget *win = gtk_application_window_new(app);
  gtk_window_set_default_size(GTK_WINDOW(win), 500, 350);
  gtk_window_set_title(GTK_WINDOW(win), "GTK Test 4");
  gtk_widget_set_app_paintable(win, FALSE);

  g_signal_connect(win, "destroy", G_CALLBACK(on_destroy), NULL);
  g_signal_connect(win, "motion-notify-event", G_CALLBACK(on_move), NULL);
  g_signal_connect(win, "button-press-event", G_CALLBACK(on_click), NULL);
  g_signal_connect(win, "draw", G_CALLBACK(on_draw), NULL);
  g_signal_connect(win, "configure-event", G_CALLBACK(on_configure), NULL);

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

// Also see https://developer.gnome.org/gtk3/stable/ch01s05.html
