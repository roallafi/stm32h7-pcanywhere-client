#include "tft_display.h"
#include "font_8x16.h"
#include "stm32h7xx_hal.h"

/* Simple RGB565 Color conversion from DOS attributes */
static const uint16_t dos_colors[16] = {
    COLOR_BLACK,         /* 0 - Black */
    COLOR_BLUE,          /* 1 - Blue */
    COLOR_GREEN,         /* 2 - Green */
    COLOR_CYAN,          /* 3 - Cyan */
    COLOR_RED,           /* 4 - Red */
    COLOR_MAGENTA,       /* 5 - Magenta */
    COLOR_YELLOW,        /* 6 - Yellow/Brown */
    COLOR_WHITE,         /* 7 - White/Light Gray */
    COLOR_GRAY,          /* 8 - Dark Gray */
    COLOR_LIGHT_BLUE,    /* 9 - Light Blue */
    COLOR_LIGHT_GREEN,   /* 10 - Light Green */
    COLOR_LIGHT_CYAN,    /* 11 - Light Cyan */
    COLOR_LIGHT_RED,     /* 12 - Light Red */
    COLOR_LIGHT_MAGENTA, /* 13 - Light Magenta */
    COLOR_LIGHT_YELLOW,  /* 14 - Light Yellow */
    COLOR_BRIGHT_WHITE   /* 15 - Bright White */
};

void TFT_Init(void) {
    /* Initialize TFT hardware (FSMC/DMA configuration handled by CubeMX) */
    TFT_Clear(COLOR_BLACK);
}

void TFT_Clear(uint16_t color) {
    uint32_t i;
    for (i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT; i++) {
        TFT_DrawPixel(i % DISPLAY_WIDTH, i / DISPLAY_WIDTH, color);
    }
}

void TFT_FillRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) {
    uint16_t i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            TFT_DrawPixel(x + j, y + i, color);
        }
    }
}

void TFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    /* This would be implemented using FSMC or DMA for actual hardware */
    /* For now, placeholder - actual implementation depends on your TFT controller */
    if (x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) return;
    
    /* Write to framebuffer or TFT controller */
    /* volatile uint16_t *fb = (uint16_t *)(TFT_BASE_ADDR); */
    /* fb[y * DISPLAY_WIDTH + x] = color; */
}

uint16_t DOS_AttributeToRGB565(uint8_t attr) {
    uint8_t bg = (attr >> 4) & 0x0F;  /* Background color (upper 4 bits) */
    return dos_colors[bg];
}

void TFT_DrawChar(uint16_t x, uint16_t y, uint8_t ascii, uint8_t attr) {
    uint16_t fg_color = dos_colors[attr & 0x0F];        /* Foreground */
    uint16_t bg_color = dos_colors[(attr >> 4) & 0x0F]; /* Background */
    
    /* Get character bitmap from font (8x16) */
    const uint8_t *char_bitmap = font_8x16[ascii];
    
    uint16_t px, py;
    
    /* Draw 8x16 character */
    for (py = 0; py < 16; py++) {
        uint8_t byte = char_bitmap[py];
        for (px = 0; px < 8; px++) {
            uint16_t color = (byte & (1 << (7 - px))) ? fg_color : bg_color;
            TFT_DrawPixel(x + px, y + py, color);
        }
    }
}

void TFT_RenderScreen(const uint8_t *screen_buffer) {
    /* screen_buffer format: [ASCII, ATTR, ASCII, ATTR, ...] */
    /* 80x25 = 2000 chars, 4000 bytes total */
    
    uint16_t col, row;
    uint16_t screen_x, screen_y;
    uint16_t buffer_idx;
    uint8_t ascii, attr;
    
    for (row = 0; row < TEXT_ROWS; row++) {
        for (col = 0; col < TEXT_COLS; col++) {
            buffer_idx = (row * TEXT_COLS + col) * 2;
            ascii = screen_buffer[buffer_idx];      /* ASCII character */
            attr = screen_buffer[buffer_idx + 1];   /* Attribute (color) */
            
            screen_x = col * 4;   /* 320/80 = 4 pixels per character */
            screen_y = row * 10;  /* 240/24 = 10 pixels per row */
            
            TFT_DrawChar(screen_x, screen_y, ascii, attr);
        }
    }
}
