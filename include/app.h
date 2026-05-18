#ifndef APP_H
#define APP_H


#include <string>

// gtk
#include <gtk/gtk.h>


struct AppState {
    std::string  text;
    bool         pressing   = false;
    int          press_row  = -1;
    int          press_col  = -1;
    double       press_x    = 0;
    double       press_y    = 0;
    double       cur_x      = 0;
    double       cur_y      = 0;
    int          hover_row  = -1;
    int          hover_col  = -1;
    GtkWidget   *drawing_area = nullptr;
};

inline AppState app;

#endif
