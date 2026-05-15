#ifndef KEYS_H
#define KEYS_H


static const int KEY_W           = 80;
static const int KEY_H           = 55;
static const int COLS            = 5;
static const int ROWS            = 4;
static const int FLICK_THRESHOLD = 20;
static const int TOP_OFFSET      = 35;

inline const char* const ACT_BACKSPACE  = "\x01";
inline const char* const ACT_DAKUTEN    = "\x02";
inline const char* const ACT_HANDAKUTEN = "\x03";
inline const char* const ACT_SMALL      = "\x04";
inline const char* const ACT_SPACE      = "\x05";

enum FlickDir {
    CENTER  = 0,
    UP      = 1,
    RIGHT   = 2,
    DOWN    = 3,
    LEFT    = 4
};

struct KeyDef {
    const char* label;
    const char* chars[5];
    const int   font_size;
};

inline const KeyDef KEYS[ROWS][COLS] = {
    {
        {"☆123", {nullptr, nullptr, nullptr, nullptr, nullptr}, 13},
        {"あ", {"あ", "う", "え", "お", "い"}, 20},
        {"か", {"か", "く", "け", "こ", "き"}, 20},
        {"さ", {"さ", "す", "せ", "そ", "し"}, 20},
        {"⌫", {ACT_BACKSPACE, nullptr, nullptr, nullptr, nullptr}, 13},
    },
    {
        {"ABC", {nullptr, nullptr, nullptr, nullptr, nullptr}, 13},
        {"た", {"た", "つ", "て", "と", "ち"}, 20},
        {"な", {"な", "ぬ", "ね", "の", "に"}, 20},
        {"は", {"は", "ふ", "へ", "ほ", "ひ"}, 20},
        {"空白", {ACT_SPACE, nullptr, nullptr, nullptr, nullptr}, 13},
    },
    {
        {"あいう", {nullptr, nullptr, nullptr, nullptr, nullptr}, 13},
        {"ま", {"ま", "む", "め", "も", "み"}, 20},
        {"や", {"や", "ゆ", nullptr, "よ", nullptr}, 20},
        {"ら", {"ら", "る", "れ", "ろ", "り"}, 20},
        {"→", {nullptr, nullptr, nullptr, nullptr, nullptr}, 20},
    },
    {
        {"◆", {nullptr, nullptr, nullptr, nullptr, nullptr}, 13},
        {"^_^", {ACT_DAKUTEN, ACT_HANDAKUTEN, ACT_SMALL, "ー", "っ"}, 13},
        {"わ",   {"わ", nullptr, nullptr, "ん", "を"}, 20},
        {"、。?!", {nullptr, nullptr, nullptr, nullptr, nullptr}, 13},
        {"→", {nullptr, nullptr, nullptr, nullptr, nullptr}, 20},
    }
};

#endif
