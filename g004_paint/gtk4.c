#include <gtk/gtk.h>
#include <cairo.h>

cairo_surface_t *surface = NULL;
int lastX, lastY;


static void clear_surface(cairo_surface_t *sur) {
  cairo_t *cr = cairo_create(sur);

  cairo_set_source_rgb(cr, 0, 0, 0);
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

static void paint(GtkWidget *widget, int x1, int y1, int x2, int y2) {
  int t;
  int w, h, hh, x, y, yy1;
  cairo_t *cr;
  cr = cairo_create(surface);
  cairo_set_source_rgb(cr, 0.3, 0.7, 1);

  if (x1 > x2) {
    t = x1; x1 = x2; x2 = t;
    t = y1; y1 = y2; y2 = t;
  }

  w = x2 - x1 + 1;
  h = y2 - y1;
  hh = h;
  if (h < 0) { h = -h; hh--; }
  h++;

  if (y1 < y2) yy1 = y1; else yy1 = y2;
  gtk_widget_queue_draw_area(widget, x1, yy1, w, h);

  if (w > h) {
    for (x = x1; x <= x2; x++) {
      y = y1 + (x - x1) * hh / w;
      cairo_rectangle(cr, x, y, 1, 1);
    }
  } else {
    if (y1 > y2) {
      t = x1; x1 = x2; x2 = t;
      t = y1; y1 = y2; y2 = t;
      w = -w;
    }
    for (y = y1; y <= y2; y++) {
      x = x1 + (y - y1) * w / h;
      cairo_rectangle(cr, x, y, 1, 1);
    }
  }

  cairo_fill(cr);
  cairo_destroy(cr);
}

static gboolean on_move(GtkWidget *win, GdkEventMotion *event, gpointer data) {
  if (event->state & GDK_BUTTON1_MASK) {
    paint(win, lastX, lastY, event->x, event->y);
    lastX = event->x;
    lastY = event->y;
  }
  return TRUE;
}

static gboolean on_click(GtkWidget *win, GdkEventButton *event, gpointer data) {
  if (event->button == GDK_BUTTON_PRIMARY) {
    gdouble x = event->x;
    gdouble y = event->y;
    paint(win, event->x, event->y, event->x, event->y);
    lastX = event->x;
    lastY = event->y;
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
  gtk_window_maximize(GTK_WINDOW(win));

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
