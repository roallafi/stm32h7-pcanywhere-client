#ifndef TFT_DISPLAY_H
#define TFT_DISPLAY_H

#include <stdint.h>
#include "config.h"

/* RGB565 Color Palette (DOS 16 colors) */
#define COLOR_BLACK         0x0000
#define COLOR_BLUE          0x001F
#define COLOR_GREEN         0x07E0
#define COLOR_CYAN          0x07FF
#define COLOR_RED           0xF800
#define COLOR_MAGENTA       0xF81F
#define COLOR_YELLOW        0xFFE0
#define COLOR_WHITE         0xFFFF
#define COLOR_GRAY          0x8410
#define COLOR_LIGHT_BLUE    0x041F
#define COLOR_LIGHT_GREEN   0x07E0
#define COLOR_LIGHT_CYAN    0x07FF
#define COLOR_LIGHT_RED     0xF800
#define COLOR_LIGHT_MAGENTA 0xF81F
#define COLOR_LIGHT_YELLOW  0xFFE0
#define COLOR_BRIGHT_WHITE  0xFFFF

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
} Rect_t;

void TFT_Init(void);
void TFT_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);
void TFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void TFT_Clear(uint16_t color);
uint16_t DOS_AttributeToRGB565(uint8_t attr);
void TFT_DrawChar(uint16_t x, uint16_t y, uint8_t ascii, uint8_t attr);
void TFT_RenderScreen(const uint8_t *screen_buffer);

#endif /* TFT_DISPLAY_H */
