#ifndef UART_SERIAL_H
#define UART_SERIAL_H

#include <stdint.h>
#include "stm32h7xx_hal.h"

typedef struct {
    UART_HandleTypeDef *huart;
    uint8_t rx_buffer[512];
    uint8_t tx_buffer[512];
    uint16_t rx_head;
    uint16_t rx_tail;
    uint16_t tx_head;
    uint16_t tx_tail;
} UART_Handle_t;

void UART_Init(UART_HandleTypeDef *huart);
void UART_SendByte(uint8_t byte);
void UART_SendData(const uint8_t *data, uint16_t len);
int UART_RecvByte(uint8_t *byte);
int UART_RecvData(uint8_t *buffer, uint16_t max_len);
int UART_DataAvailable(void);
void UART_IRQHandler(void);

#endif /* UART_SERIAL_H */
