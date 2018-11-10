
#ifndef _SCALER_H
#define _SCALER_H

#include <stdint.h>

void bitmap_scale(int startx, int starty, int viswidth, int visheight, int newwidth, int newheight,int pitch, uint16_t *src, uint16_t *dst);
#endif
