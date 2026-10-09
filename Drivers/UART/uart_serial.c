#include "uart_serial.h"
#include "config.h"

static UART_Handle_t uart_handle;

void UART_Init(UART_HandleTypeDef *huart) {
    uart_handle.huart = huart;
    uart_handle.rx_head = 0;
    uart_handle.rx_tail = 0;
    uart_handle.tx_head = 0;
    uart_handle.tx_tail = 0;
    
    /* Enable UART RX interrupt */
    HAL_UART_Receive_IT(huart, (uint8_t *)&uart_handle.rx_buffer[0], 1);
}

void UART_SendByte(uint8_t byte) {
    while (HAL_UART_GetState(uart_handle.huart) != HAL_UART_STATE_READY) {
        /* Wait */
    }
    HAL_UART_Transmit(uart_handle.huart, &byte, 1, 100);
}

void UART_SendData(const uint8_t *data, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        UART_SendByte(data[i]);
    }
}

int UART_RecvByte(uint8_t *byte) {
    if (uart_handle.rx_head == uart_handle.rx_tail) {
        return -1;  /* No data available */
    }
    
    *byte = uart_handle.rx_buffer[uart_handle.rx_tail];
    uart_handle.rx_tail = (uart_handle.rx_tail + 1) % UART_RX_BUFFER_SIZE;
    return 0;
}

int UART_RecvData(uint8_t *buffer, uint16_t max_len) {
    uint16_t count = 0;
    
    while (count < max_len) {
        if (UART_RecvByte(&buffer[count]) == 0) {
            count++;
        } else {
            break;
        }
    }
    
    return count;
}

int UART_DataAvailable(void) {
    return uart_handle.rx_head != uart_handle.rx_tail;
}

void UART_IRQHandler(void) {
    uint8_t data = 0;
    
    if (HAL_UART_Receive(uart_handle.huart, &data, 1, 0) == HAL_OK) {
        uart_handle.rx_buffer[uart_handle.rx_head] = data;
        uart_handle.rx_head = (uart_handle.rx_head + 1) % UART_RX_BUFFER_SIZE;
    }
}
