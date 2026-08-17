#ifndef OLED_FONT_H
#define OLED_FONT_H

#include <stdint.h>

#define OLED_FONT8X16_WIDTH   8U
#define OLED_FONT8X16_HEIGHT  16U
#define OLED_FONT8X16_FIRST   32U
#define OLED_FONT8X16_LAST    126U

extern const uint8_t OLED_Font8x16[][16];

const uint8_t *OLED_Font8x16_Get(char ch);

#endif /* OLED_FONT_H */
