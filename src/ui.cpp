#include <cmath>

// pango
#include <pango/pangocairo.h>

// original
#include "ui.h"
#include "app.h"
#include "keys.h"
#include "flick.h"
#include "uinput_dev.h"


struct FlickLabelPos {
    FlickDir d;
    double cx, cy;
};

static void roundedRect(cairo_t *cr, double x, double y, double w, double h, double r) {
    cairo_move_to(cr, x + r, y);
    cairo_line_to(cr, x + w - r, y);
    cairo_arc(cr, x + w - r, y + r, r, -M_PI/2, 0);
    cairo_line_to(cr, x + w, y + h - r);
    cairo_arc(cr, x + w - r, y + h - r, r, 0, M_PI/2);
    cairo_line_to(cr, x + r, y + h);
    cairo_arc(cr, x + r, y + h - r, r, M_PI/2, M_PI);
    cairo_line_to(cr, x, y + r);
    cairo_arc(cr, x + r, y + r, r, M_PI, 3*M_PI/2);
    cairo_close_path(cr);
}

static void drawCallout(cairo_t *cr, double kx, double ky, KeyDef key, FlickDir d) {
    if (d == CENTER) return;

    const double pw = KEY_W - 3, ph = KEY_H - 3, pr = 12, pt = 10;
    double px, py;
    if (d == UP) {
        px = kx - pw/2;
        py = ky - ph/2 - ph + 3;
    }
    else if (d == DOWN) {
        px = kx - pw/2;
        py = ky + KEY_H/2 - 3;
    }
    else if (d == RIGHT) {
        px = kx + KEY_W/2;
        py = ky - ph/2;
    }
    else if (d == LEFT) {
        px = kx - KEY_W/2 - pw;
        py = ky - ph/2;
    }

    cairo_new_path(cr);
    switch (d) {
        case UP:
            cairo_move_to(cr, px + pr, py);
            cairo_line_to(cr, px + pw - pr, py);
            cairo_arc(cr, px + pw - pr, py + pr, pr, -M_PI/2, 0);
            cairo_line_to(cr, px + pw, py + ph - pr);
            cairo_line_to(cr, px + pw/2, py + ph + pt);
            cairo_line_to(cr, px, py + ph - pr);
            cairo_line_to(cr, px, py + pr);
            cairo_arc(cr, px + pr, py + pr, pr, M_PI, 3*M_PI/2);
            break;
        case DOWN:
            cairo_move_to(cr, px, py + pr);
            cairo_line_to(cr, px + pw/2, py - pt);
            cairo_line_to(cr, px + pw, py + pr);
            cairo_line_to(cr, px + pw, py + ph - pr);
            cairo_arc(cr, px + pw - pr, py + ph - pr, pr, 0, M_PI/2);
            cairo_line_to(cr, px + pr, py + ph);
            cairo_arc(cr, px + pr, py + ph - pr, pr,  M_PI/2, M_PI);
            cairo_line_to(cr, px, py + pr);
            break;
        case RIGHT:
            cairo_move_to(cr, px + pr, py);
            cairo_line_to(cr, px + pw - pr, py);
            cairo_arc(cr, px + pw - pr, py + pr, pr, -M_PI/2, 0);
            cairo_line_to(cr, px + pw, py + ph - pr);
            cairo_arc(cr, px + pw - pr, py + ph - pr, pr, 0, M_PI/2);
            cairo_line_to(cr, px + pr, py + ph);
            cairo_line_to(cr, px - pt, py + ph/2);
            cairo_line_to(cr, px + pr, py);
            break;
        case LEFT:
            cairo_move_to(cr, px + pr, py);
            cairo_line_to(cr, px + pw - pr, py);
            cairo_line_to(cr, px + pw + pt, py + ph/2);
            cairo_line_to(cr, px + pw - pr, py + ph);
            cairo_line_to(cr, px + pr, py + ph);
            cairo_arc(cr, px + pr, py + ph - pr, pr, M_PI/2, M_PI);
            cairo_line_to(cr, px, py + pr);
            cairo_arc(cr, px + pr, py + pr, pr, M_PI, 3*M_PI/2);
            break;
        default: break;
    }
    cairo_close_path(cr);

    cairo_set_source_rgb(cr, 0.7, 0.7, 0.7);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    pangoDrawText(cr, key.chars[d], px, py, pw, ph, key.font_size);
}

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
    int row = (int)((event->y - TOP_OFFSET) / KEY_H);
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
    cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);
    cairo_paint(cr);

    FlickDir live_dir = CENTER;
    if (app.pressing) {
        live_dir = getDirection(app.cur_x - app.press_x,
                                app.cur_y - app.press_y);
    }

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            double x = c * KEY_W;
            double y = r * KEY_H + TOP_OFFSET;
            bool pressed = (app.pressing && (app.press_row == r) && (app.press_col == c));

            if (pressed) {
                cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);
            } else {
                cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
            }
            roundedRect(cr, x+3, y+3, KEY_W-6, KEY_H-6, 10);
            cairo_fill(cr);

            const KeyDef &key = KEYS[r][c];
            cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
            pangoDrawText(cr, key.label, x, y, KEY_W, KEY_H, key.font_size);
        }
    }

    if (app.pressing) {
        const KeyDef key = KEYS[app.press_row][app.press_col];
        const char *ch = key.chars[live_dir];
        if (ch && ch != ACT_BACKSPACE && ch != ACT_DAKUTEN && ch != ACT_HANDAKUTEN && ch != ACT_SMALL) {
            double kx = app.press_col * KEY_W + KEY_W / 2.0;
            double ky = app.press_row * KEY_H + KEY_H / 2.0 + TOP_OFFSET;
            drawCallout(cr, kx, ky, key, live_dir);
        }
    }

    return FALSE;
}

void onTabClicked(GtkButton*, gpointer) {
    uinputSendChar(0x0009);
}

void onSpaceClicked(GtkButton*, gpointer) {
    app.text += "　";
    uinputSendChar(0x0020);
}
