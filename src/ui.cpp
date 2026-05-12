#include <cmath>

// pango
#include <pango/pangocairo.h>

// original
#include "ui.h"
#include "app.h"
#include "keys.h"
#include "flick.h"


void pangoDrawText(cairo_t *cr, const char *text, double x, double y,
                   double box_w, double box_h, int font_size, bool center) {
    PangoLayout *layout = pango_cairo_create_layout(cr);
    char font_desc_str[32];
    snprintf(font_desc_str, sizeof(font_desc_str), "Sans %d", font_size);
    PangoFontDescription *fd = pango_font_description_from_string(font_desc_str);
    pango_layout_set_font_description(layout, fd);
    pango_font_description_free(fd);
    pango_layout_set_text(layout, text, -1);

    if (center) {
        int tw, th;
        pango_layout_get_pixel_size(layout, &tw, &th);
        cairo_move_to(cr, x + (box_w - tw) / 2.0, y + (box_h - th) / 2.0);
    } else {
        cairo_move_to(cr, x, y);
    }
    pango_cairo_show_layout(cr, layout);
    g_object_unref(layout);
}

gboolean onButtonPress(GtkWidget *widget, GdkEventButton *event, gpointer) {
    if (event->button != 1) return FALSE;
    int col = (int)(event->x / KEY_W);
    int row = (int)(event->y / KEY_H);
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) return FALSE;

    app.pressing  = true;
    app.press_row = row;
    app.press_col = col;
    app.press_x   = event->x;
    app.press_y   = event->y;
    app.cur_x     = event->x;
    app.cur_y     = event->y;
    gtk_widget_queue_draw(widget);
    return TRUE;
}

gboolean onMotion(GtkWidget *widget, GdkEventMotion *event, gpointer) {
    if (!app.pressing) return FALSE;
    app.cur_x = event->x;
    app.cur_y = event->y;
    gtk_widget_queue_draw(widget);
    return TRUE;
}

gboolean onButtonRelease(GtkWidget *widget, GdkEventButton *event, gpointer) {
    if (!app.pressing || event->button != 1) return FALSE;
    double dx = event->x - app.press_x;
    double dy = event->y - app.press_y;
    applyKey(app.press_row, app.press_col, getDirection(dx, dy));
    app.pressing  = false;
    app.press_row = -1;
    app.press_col = -1;
    gtk_widget_queue_draw(widget);
    return TRUE;
}

gboolean onDraw(GtkWidget*, cairo_t *cr, gpointer) {
    FlickDir live_dir = CENTER;
    if (app.pressing) {
        live_dir = getDirection(app.cur_x - app.press_x,
                                app.cur_y - app.press_y);
    }

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            double x = c * KEY_W;
            double y = r * KEY_H;
            bool pressed = (app.pressing && app.press_row == r && app.press_col == c);

            if (pressed) {
                cairo_set_source_rgb(cr, 0.55, 0.78, 1.0);
            } else if (r == ROWS-1 && c == COLS-1) {
                cairo_set_source_rgb(cr, 1.0, 0.68, 0.68);
            } else if (r == ROWS-1 && c == 0) {
                cairo_set_source_rgb(cr, 0.95, 0.93, 0.68);
            } else {
                cairo_set_source_rgb(cr, 0.88, 0.88, 0.88);
            }
            cairo_rectangle(cr, x+1, y+1, KEY_W-2, KEY_H-2);
            cairo_fill(cr);

            cairo_set_source_rgb(cr, 0.55, 0.55, 0.55);
            cairo_set_line_width(cr, 1.0);
            cairo_rectangle(cr, x+0.5, y+0.5, KEY_W-1, KEY_H-1);
            cairo_stroke(cr);

            const KeyDef &key = KEYS[r][c];

            if (pressed) {
                struct { FlickDir d; double cx, cy; } pos[] = {
                    {UP,    x + KEY_W/2.0, y + 8},
                    {RIGHT, x + KEY_W - 8, y + KEY_H/2.0 - 8},
                    {DOWN,  x + KEY_W/2.0, y + KEY_H - 24},
                    {LEFT,  x + 4,         y + KEY_H/2.0 - 8},
                };
                for (auto &p : pos) {
                    const char *ch = key.chars[p.d];
                    if (!ch || ch == ACT_BACKSPACE || ch == ACT_DAKUTEN ||
                        ch == ACT_HANDAKUTEN || ch == ACT_SMALL) continue;
                    if (p.d == live_dir)
                        cairo_set_source_rgb(cr, 0.85, 0.25, 0.05);
                    else
                        cairo_set_source_rgb(cr, 0.25, 0.25, 0.25);
                    pangoDrawText(cr, ch, p.cx - 8, p.cy, 16, 20, 12, false);
                }

                if (live_dir == CENTER)
                    cairo_set_source_rgb(cr, 0.85, 0.25, 0.05);
                else
                    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
                pangoDrawText(cr, key.label, x, y, KEY_W, KEY_H, 20);
            } else {
                cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
                pangoDrawText(cr, key.label, x, y, KEY_W, KEY_H, 20);
            }
        }
    }

    if (app.pressing) {
        double dx = app.cur_x - app.press_x;
        double dy = app.cur_y - app.press_y;
        double dist = std::sqrt(dx*dx + dy*dy);
        if (dist > 8) {
            double kx = app.press_col * KEY_W + KEY_W / 2.0;
            double ky = app.press_row * KEY_H + KEY_H / 2.0;
            double scale = std::min(dist, KEY_W / 2.0 - 12) / dist;
            double ex = kx + dx * scale;
            double ey = ky + dy * scale;
            double angle = std::atan2(dy, dx);

            cairo_set_source_rgba(cr, 0.15, 0.45, 0.95, 0.8);
            cairo_set_line_width(cr, 3);
            cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND);
            cairo_move_to(cr, kx, ky);
            cairo_line_to(cr, ex, ey);
            cairo_stroke(cr);

            cairo_move_to(cr, ex, ey);
            cairo_line_to(cr, ex - 10 * std::cos(angle - 0.45), ey - 10 * std::sin(angle - 0.45));
            cairo_move_to(cr, ex, ey);
            cairo_line_to(cr, ex - 10 * std::cos(angle + 0.45), ey - 10 * std::sin(angle + 0.45));
            cairo_stroke(cr);
        }
    }

    return FALSE;
}

void onClearClicked(GtkButton*, gpointer) {
    app.text.clear();
    gtk_entry_set_text(GTK_ENTRY(app.entry), "");
}

void onSpaceClicked(GtkButton*, gpointer) {
    app.text += "　";
    gtk_entry_set_text(GTK_ENTRY(app.entry), app.text.c_str());
    gtk_editable_set_position(GTK_EDITABLE(app.entry), -1);
}
