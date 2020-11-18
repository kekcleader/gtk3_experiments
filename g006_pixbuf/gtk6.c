#include <gtk/gtk.h>
#include <cairo.h>

#define WW 640
#define HH 350

struct {
  GdkPixbuf *tileset;
  guint8 data[3 * WW * HH];
  gboolean need_redraw;
} glob;

void clear_pixbuf() {
  int i;
  for (i = 0; i < sizeof(glob.data); i++) {
    glob.data[i] = 0;
  }
  glob.need_redraw = TRUE;
}

static void draw_pixbuf(cairo_t *cr, GdkPixbuf *pixbuf, int src_x, int src_y,
    int dest_x, int dest_y, int w, int h) {
  cairo_save(cr);
  cairo_translate(cr, dest_x, dest_y);
  gdk_cairo_set_source_pixbuf(cr, pixbuf, -src_x, -src_y);
  cairo_rectangle(cr, 0, 0, w, h);
  cairo_fill(cr);
  cairo_restore(cr);
}

static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
  GBytes *bytes;

  //cairo_set_source_rgb(cr, 0.2, 0.3, 0.4);
  //cairo_rectangle(cr, 0, 0, 500, 350);
  //cairo_fill(cr);

  if (glob.need_redraw) {
    if (glob.tileset) {
      g_object_unref(glob.tileset);
    }
    bytes = g_bytes_new(glob.data, sizeof(glob.data));
    glob.tileset = gdk_pixbuf_new_from_bytes(bytes, GDK_COLORSPACE_RGB,
      FALSE, 8, WW, HH, WW * 3);
    glob.need_redraw = FALSE;
  }

  draw_pixbuf(cr, glob.tileset, 0, 0, 0, 0, WW, HH);

  return TRUE;
}

static void paint(GtkWidget *win, gint x, gint y) {
  gint i;
  i = 3 * (y * WW + x);
  if (i < sizeof(glob.data)) {
    glob.data[i] = 255;
    glob.need_redraw = TRUE;
    gtk_widget_queue_draw_area(win, x, y, 1, 1);
  }
}

static gboolean on_move(GtkWidget *win, GdkEventMotion *event, gpointer data) {
  if (event->state & GDK_BUTTON1_MASK) {
    paint(win, event->x, event->y);
  }
  return TRUE;
}

static gboolean on_click(GtkWidget *win, GdkEventButton *event, gpointer data) {
  if (event->button == GDK_BUTTON_PRIMARY) {
    paint(win, event->x, event->y);
  } else if (event->button == GDK_BUTTON_SECONDARY) {
    clear_pixbuf();
    gtk_widget_queue_draw(win);
  }
  return TRUE;
}

static void on_activate(GtkApplication *app) {
  GtkWidget *win = gtk_application_window_new(app);
  gtk_window_set_default_size(GTK_WINDOW(win), WW, HH);
  gtk_window_set_title(GTK_WINDOW(win), "GTK Test 3");
  gtk_widget_set_app_paintable(win, FALSE);

  g_signal_connect(win, "draw", G_CALLBACK(on_draw), NULL);
  g_signal_connect(win, "motion-notify-event", G_CALLBACK(on_move), NULL);
  g_signal_connect(win, "button-press-event", G_CALLBACK(on_click), NULL);

  gtk_widget_show_all(win);
}

int main(int argc, char *argv[]) {
  clear_pixbuf();
  glob.tileset = NULL;

  GtkApplication *app = gtk_application_new("com.example.GtkApplication", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
  return g_application_run(G_APPLICATION(app), argc, argv);
}
