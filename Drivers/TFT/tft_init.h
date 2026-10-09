#ifndef TFT_INIT_H
#define TFT_INIT_H

#include "stm32h7xx_hal.h"

void TFT_GPIO_Init(void);
void TFT_FSMC_Config(void);
void TFT_Reset(void);
void TFT_WriteCommand(uint16_t cmd);
void TFT_WriteData(uint16_t data);
void TFT_SetWindow(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

#endif /* TFT_INIT_H */
