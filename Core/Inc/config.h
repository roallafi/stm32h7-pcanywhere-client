#ifndef CONFIG_H
#define CONFIG_H

/* STM32H7 Configuration */
#define STM32H7_ENABLED 1

/* UART Configuration */
#define UART_PORT           USART1
#define UART_BAUD_RATE      9600
#define UART_RX_BUFFER_SIZE 512
#define UART_TX_BUFFER_SIZE 512

/* USB Configuration */
#define USB_HOST_ENABLED    1
#define USB_MAX_DEVICES     1

/* Display Configuration */
#define DISPLAY_WIDTH       320
#define DISPLAY_HEIGHT      240
#define DISPLAY_BPP         16  /* RGB565 */
#define TEXT_COLS           80
#define TEXT_ROWS           25
#define CHAR_WIDTH          4   /* 320/80 */
#define CHAR_HEIGHT         10  /* 240/24 */

/* Screen Buffer */
#define SCREEN_BUFFER_SIZE  4000  /* 80*25*2 */

/* FreeRTOS Configuration */
#define TASK_STACK_SIZE     512
#define SERIAL_TASK_PRIO    2
#define KEYBOARD_TASK_PRIO  3
#define DISPLAY_TASK_PRIO   1

/* Protocol */
#define MSG_HANDSHAKE       0x01
#define MSG_ACK             0x02
#define MSG_SCREEN          0x07
#define MSG_KEYBOARD        0x08
#define MSG_DISCONNECT      0x09
#define MSG_PING            0x0A

#define PACKET_SIZE         256

#endif /* CONFIG_H */
