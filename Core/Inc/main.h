#ifndef MAIN_H
#define MAIN_H

#include "stm32h7xx_hal.h"
#include "config.h"

/* Global Handle Structures */
extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern HCD_HandleTypeDef hhcd_USB_OTG_HS;

/* Function Declarations */
void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_USART1_UART_Init(void);
void MX_DMA_Init(void);
void MX_FSMC_Init(void);
void MX_DMA2D_Init(void);
void MX_USB_HOST_Init(void);
void MX_FreeRTOS_Init(void);

/* LED Status Indicators */
#define LED_GREEN_PORT  GPIOB
#define LED_GREEN_PIN   GPIO_PIN_0
#define LED_RED_PORT    GPIOB
#define LED_RED_PIN     GPIO_PIN_1
#define LED_BLUE_PORT   GPIOB
#define LED_BLUE_PIN    GPIO_PIN_14

void LED_Init(void);
void LED_SetGreen(uint8_t state);
void LED_SetRed(uint8_t state);
void LED_SetBlue(uint8_t state);

#endif /* MAIN_H */
