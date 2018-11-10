
#ifndef _SCALER_H
#define _SCALER_H

#include <stdint.h>

void gb_upscale_320x240(uint32_t *to, uint32_t *from);

void gb_upscale_320x240_for_400x240(uint32_t *dst, uint32_t *src);
void gb_upscale_320x272_for_480x272(uint32_t *dst, uint32_t *src);
void gb_upscale_400x240(uint32_t *dst, uint32_t *src);
void gb_upscale_480x272(uint32_t *dst, uint32_t *src);

#endif
