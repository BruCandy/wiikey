// gtk
#include <gtk/gtk.h>

// original
#include "app.h"
#include "keys.h"
#include "ui.h"
#include "uinput_dev.h"


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    uinputInit();

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "wiikey");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_accept_focus(GTK_WINDOW(window), FALSE);
    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_container_set_border_width(GTK_CONTAINER(window), 4);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    GtkWidget *hbox      = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4);
    GtkWidget *btn_space = gtk_button_new_with_label("スペース");
    GtkWidget *btn_tab   = gtk_button_new_with_label("タブ");
    gtk_box_pack_start(GTK_BOX(hbox), btn_space, TRUE,  TRUE,  0);
    gtk_box_pack_start(GTK_BOX(hbox), btn_tab,   FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);
    g_signal_connect(btn_space, "clicked", G_CALLBACK(onSpaceClicked), nullptr);
    g_signal_connect(btn_tab,   "clicked", G_CALLBACK(onTabClicked),   nullptr);

    app.drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(app.drawing_area, COLS * KEY_W, ROWS * KEY_H);
    gtk_widget_add_events(app.drawing_area,
        GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK);
    g_signal_connect(app.drawing_area, "draw",                 G_CALLBACK(onDraw),          nullptr);
    g_signal_connect(app.drawing_area, "button-press-event",   G_CALLBACK(onButtonPress),   nullptr);
    g_signal_connect(app.drawing_area, "button-release-event", G_CALLBACK(onButtonRelease), nullptr);
    g_signal_connect(app.drawing_area, "motion-notify-event",  G_CALLBACK(onMotion),        nullptr);
    gtk_box_pack_start(GTK_BOX(vbox), app.drawing_area, FALSE, FALSE, 0);

    gtk_widget_show_all(window);
    gtk_main();

    uinputClose();
    return 0;
}
