// gtk
#include <gtk/gtk.h>

// original
#include "app.h"
#include "keys.h"
#include "ui.h"
#include "uinput_dev.h"


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    if (!uinputInit()) {
        perror("Failed to init uinput");
        return 0;
    }

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "wiikey");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_accept_focus(GTK_WINDOW(window), FALSE);
    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);

    app.drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(app.drawing_area, COLS * KEY_W, ROWS * KEY_H);
    gtk_widget_add_events(app.drawing_area, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK);
    g_signal_connect(app.drawing_area, "draw",                 G_CALLBACK(onDraw),          nullptr);
    g_signal_connect(app.drawing_area, "button-press-event",   G_CALLBACK(onButtonPress),   nullptr);
    g_signal_connect(app.drawing_area, "button-release-event", G_CALLBACK(onButtonRelease), nullptr);
    g_signal_connect(app.drawing_area, "motion-notify-event",  G_CALLBACK(onMotion),        nullptr);
    gtk_container_add(GTK_CONTAINER(window), app.drawing_area);

    gtk_widget_show_all(window);
    gtk_main();

    uinputClose();
    return 0;
}
