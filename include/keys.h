#ifndef KEYS_H
#define KEYS_H


static const int KEY_W           = 90;
static const int KEY_H           = 80;
static const int COLS            = 3;
static const int ROWS            = 4;
static const int FLICK_THRESHOLD = 20;

inline const char* const ACT_BACKSPACE  = "\x01";
inline const char* const ACT_DAKUTEN    = "\x02";
inline const char* const ACT_HANDAKUTEN = "\x03";
inline const char* const ACT_SMALL      = "\x04";

enum FlickDir { CENTER = 0, UP = 1, RIGHT = 2, DOWN = 3, LEFT = 4 };

struct KeyDef {
    const char* label;
    const char* chars[5];
};

inline const KeyDef KEYS[ROWS][COLS] = {
    {
        {"あ", {"あ", "う", "え", "お", "い"}},
        {"か", {"か", "く", "け", "こ", "き"}},
        {"さ", {"さ", "す", "せ", "そ", "し"}},
    },
    {
        {"た", {"た", "つ", "て", "と", "ち"}},
        {"な", {"な", "ぬ", "ね", "の", "に"}},
        {"は", {"は", "ふ", "へ", "ほ", "ひ"}},
    },
    {
        {"ま", {"ま", "む", "め", "も", "み"}},
        {"や", {"や", "ゆ", nullptr, "よ", nullptr}},
        {"ら", {"ら", "る", "れ", "ろ", "り"}},
    },
    {
        {"小/゛", {ACT_DAKUTEN, ACT_HANDAKUTEN, ACT_SMALL, "ー", "っ"}},
        {"わ",   {"わ", nullptr, nullptr, "ん", "を"}},
        {"⌫",   {ACT_BACKSPACE, nullptr, nullptr, nullptr, nullptr}},
    }
};

#endif
