#ifndef UTF8_H
#define UTF8_H


#include <string>
#include <cstdint>

size_t      utf8LastCharStart(const std::string& s);
std::string utf8RemoveLast(const std::string& s);
uint32_t    utf8ToCp(const std::string& s, size_t pos = 0);
std::string cpToUtf8(uint32_t cp);

#endif
