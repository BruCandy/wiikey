#include <cmath>

// original
#include "flick.h"
#include "app.h"
#include "modifier.h"
#include "utf8.h"
#include "uinput_dev.h"


FlickDir getDirection(double dx, double dy) {
    if (std::sqrt(dx*dx + dy*dy) < FLICK_THRESHOLD) return CENTER;
    if (std::abs(dx) > std::abs(dy)) return dx > 0 ? RIGHT : LEFT;
    return dy < 0 ? UP : DOWN;
}

static void sendModifierResult(const std::string& old_text) {
    if (app.text == old_text) return;
    uinputSendBackspace();
    size_t pos = utf8LastCharStart(app.text);
    uinputSendChar(utf8ToCp(app.text, pos));
}

void applyKey(int row, int col, FlickDir dir) {
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) return;
    const char *ch = KEYS[row][col].chars[dir];
    if (!ch) return;

    std::string old_text = app.text;

    if (ch == ACT_BACKSPACE) {
        app.text = utf8RemoveLast(app.text);
        uinputSendBackspace();
    } else if (ch == ACT_DAKUTEN) {
        app.text = applyDakuten(app.text);
        sendModifierResult(old_text);
    } else if (ch == ACT_HANDAKUTEN) {
        app.text = applyHandakuten(app.text);
        sendModifierResult(old_text);
    } else if (ch == ACT_SMALL) {
        app.text = applySmall(app.text);
        sendModifierResult(old_text);
    } else {
        app.text += ch;
        uinputSendChar(utf8ToCp(std::string(ch)));
    }

    gtk_entry_set_text(GTK_ENTRY(app.entry), app.text.c_str());
    gtk_editable_set_position(GTK_EDITABLE(app.entry), -1);
}
