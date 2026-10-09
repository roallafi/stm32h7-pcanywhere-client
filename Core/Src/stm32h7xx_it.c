#include "stm32h7xx.h"

extern void SystemInit(void);

void USART1_IRQHandler(void) {
    extern void UART_IRQHandler(void);
    UART_IRQHandler();
}

void USB_OTG_HS_IRQHandler(void) {
    extern void HAL_HCD_IRQHandler(HCD_HandleTypeDef *);
    extern HCD_HandleTypeDef hhcd_USB_OTG_HS;
    HAL_HCD_IRQHandler(&hhcd_USB_OTG_HS);
}

void DMA2_Stream1_IRQHandler(void) {
    extern DMA_HandleTypeDef hdma_usart1_rx;
    HAL_DMA_IRQHandler(&hdma_usart1_rx);
}

void OTG_HS_EP1_IN_IRQHandler(void) {
    extern void HAL_HCD_IRQHandler(HCD_HandleTypeDef *);
    extern HCD_HandleTypeDef hhcd_USB_OTG_HS;
    HAL_HCD_IRQHandler(&hhcd_USB_OTG_HS);
}

void OTG_HS_EP1_OUT_IRQHandler(void) {
    extern void HAL_HCD_IRQHandler(HCD_HandleTypeDef *);
    extern HCD_HandleTypeDef hhcd_USB_OTG_HS;
    HAL_HCD_IRQHandler(&hhcd_USB_OTG_HS);
}
