// original
#include "utf8.h"


size_t utf8LastCharStart(const std::string& s) {
    if (s.empty()) return 0;
    size_t i = s.size();
    while (i > 0 && (s[i-1] & 0xC0) == 0x80) --i;
    if (i > 0) --i;
    return i;
}

std::string utf8RemoveLast(const std::string& s) {
    if (s.empty()) return s;
    return s.substr(0, utf8LastCharStart(s));
}

uint32_t utf8ToCp(const std::string& s, size_t pos) {
    if (pos >= s.size()) return 0;
    uint8_t c = (uint8_t)s[pos];
    if (c < 0x80) return c;
    if (c < 0xE0) return ((c & 0x1F) << 6)  | ((uint8_t)s[pos+1] & 0x3F);
    if (c < 0xF0) return ((c & 0x0F) << 12) | (((uint8_t)s[pos+1] & 0x3F) << 6) | ((uint8_t)s[pos+2] & 0x3F);
    return ((c & 0x07) << 18) | (((uint8_t)s[pos+1] & 0x3F) << 12) | (((uint8_t)s[pos+2] & 0x3F) << 6) | ((uint8_t)s[pos+3] & 0x3F);
}

std::string cpToUtf8(uint32_t cp) {
    std::string r;
    if (cp < 0x80) {
        r += (char)cp;
    } else if (cp < 0x800) {
        r += (char)(0xC0 | (cp >> 6));
        r += (char)(0x80 | (cp & 0x3F));
    } else if (cp < 0x10000) {
        r += (char)(0xE0 | (cp >> 12));
        r += (char)(0x80 | ((cp >> 6) & 0x3F));
        r += (char)(0x80 | (cp & 0x3F));
    }
    return r;
}
