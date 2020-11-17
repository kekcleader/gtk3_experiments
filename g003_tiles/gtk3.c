#include <gtk/gtk.h>
#include <cairo.h>

struct {
  GdkPixbuf *tileset;
} glob;

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
  cairo_set_source_rgb(cr, 0.2, 0.3, 0.4);
  cairo_rectangle(cr, 0, 0, 500, 350);
  cairo_fill(cr);

  draw_pixbuf(cr, glob.tileset, 256, 0, 64+50, 50-32, 128, 128);
  draw_pixbuf(cr, glob.tileset, 256, 0, 128+64+50, 50-32, 128, 128);
  draw_pixbuf(cr, glob.tileset, 0, 0, 50, 50, 128, 128);
  draw_pixbuf(cr, glob.tileset, 128, 128, 128+50, 50, 128, 128);

  return TRUE;
}

static void on_activate(GtkApplication *app) {
  GtkWidget *win = gtk_application_window_new(app);
  gtk_window_set_default_size(GTK_WINDOW(win), 500, 350);
  gtk_window_set_title(GTK_WINDOW(win), "GTK Test 3");
  gtk_widget_set_app_paintable(win, FALSE);

  g_signal_connect(win, "draw", G_CALLBACK(on_draw), NULL);

  glob.tileset = gdk_pixbuf_new_from_file("tileset.png", NULL);
  //!FIXME destroy pixbuf later?

  gtk_widget_show_all(win);
}

int main(int argc, char *argv[]) {
  GtkApplication *app = gtk_application_new("com.example.GtkApplication", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
  return g_application_run(G_APPLICATION(app), argc, argv);
}
