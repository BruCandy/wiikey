#ifndef UINPUT_DEV_H
#define UINPUT_DEV_H


#include <cstdint>

bool uinputInit();
void uinputClose();
void uinputSendChar(uint32_t codepoint);
void uinputSendBackspace();

#endif
