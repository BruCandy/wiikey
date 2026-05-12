#ifndef FLICK_H
#define FLICK_H


// original
#include "keys.h"


FlickDir getDirection(double dx, double dy);
void     applyKey(int row, int col, FlickDir dir);

#endif
