#ifndef UI_H
#define UI_H


// gtk
#include <gtk/gtk.h>

// cairo
#include <cairo/cairo.h>


void     pangoDrawText(cairo_t *cr, const char *text, double x, double y,
                       double box_w, double box_h, int font_size, bool center = true);

gboolean onButtonPress(GtkWidget *widget, GdkEventButton *event, gpointer data);
gboolean onMotion(GtkWidget *widget, GdkEventMotion *event, gpointer data);
gboolean onLeaveNotify(GtkWidget *widget, GdkEventCrossing *event, gpointer data);
gboolean onButtonRelease(GtkWidget *widget, GdkEventButton *event, gpointer data);
gboolean onDraw(GtkWidget *widget, cairo_t *cr, gpointer data);
void     onTabClicked(GtkButton *button, gpointer data);
void     onSpaceClicked(GtkButton *button, gpointer data);

#endif
