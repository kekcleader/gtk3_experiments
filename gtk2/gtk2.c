#include <gtk/gtk.h>
#include <cairo.h>

struct {
  int count;
  double coordx[100];
  double coordy[100];
} glob;

static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data) {
  cairo_set_source_rgb(cr, 0.8, 0.1, 0);
  
  cairo_rectangle(cr, 0, 0, 500, 350);
  cairo_fill(cr);

  cairo_set_source_rgb(cr, 1, 0.9, 0.1);
  cairo_set_line_width(cr, 1.5);

  int i, j;
  for (i = 0; i <= glob.count - 1; i++ ) {
    for (j = 0; j <= glob.count - 1; j++ ) {
      cairo_move_to(cr, glob.coordx[i], glob.coordy[i]);
      cairo_line_to(cr, glob.coordx[j], glob.coordy[j]);
    }
  }

  cairo_stroke(cr);

  return TRUE;
}

static gboolean on_click(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
  if (event->button == 1) {
    if (glob.count < 99) {
      glob.coordx[glob.count] = event->x;
      glob.coordy[glob.count] = event->y;
      glob.count++;
      gtk_widget_queue_draw(widget);
    }
  } else if (event->button == 3) {
    glob.count = 0;
    gtk_widget_queue_draw(widget);
  }
  return TRUE;
}

static void on_activate(GtkApplication *app) {
  GtkWidget *win = gtk_application_window_new(app);
  gtk_window_set_default_size(GTK_WINDOW(win), 500, 350);
  gtk_window_set_title(GTK_WINDOW(win), "GTK Test 2");
  gtk_widget_set_app_paintable(win, FALSE);

  g_signal_connect(win, "draw", G_CALLBACK(on_draw), NULL);
  g_signal_connect(win, "button-press-event", G_CALLBACK(on_click), NULL);

  gtk_widget_show_all(win);
}

int main(int argc, char *argv[]) {
  glob.count = 0;

  GtkApplication *app = gtk_application_new("com.example.GtkApplication", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);
  return g_application_run(G_APPLICATION(app), argc, argv);
}
