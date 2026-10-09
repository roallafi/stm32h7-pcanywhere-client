#include "keyboard_task.h"
#include "protocol.h"
#include "uart_serial.h"
#include "config.h"

extern QueueHandle_t keyboard_queue;

/* Simple USB HID keyboard scan codes mapping */
static const char keyboard_map[128] = {
    0,    0,    0,    0,   'A',  'B',  'C',  'D',   /* 0-7 */
    'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',   /* 8-15 */
    'M',  'N',  'O',  'P',  'Q',  'R',  'S',  'T',   /* 16-23 */
    'U',  'V',  'W',  'X',  'Y',  'Z',  '1',  '2',   /* 24-31 */
    '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',   /* 32-39 */
    13,   27,   8,    9,   ' ',  '-',  '=',  '[',   /* 40-47 */
    ']',  '\\', 0,   ';',  '\'', '`',  ',',  '.',   /* 48-55 */
    '/',  0,    0,    0,    0,    0,    0,    0,     /* 56-63 */
    /* ... more keys can be added ... */
};

static uint8_t last_keys[6] = {0};

void KeyboardTask_Init(void) {
    /* Initialize USB Host HID keyboard */
    /* This would be configured via STM32CubeMX USB Host settings */
}

void KeyboardTask(void *pvParameters) {
    uint8_t key_code = 0;
    Packet_t pkt;
    
    KeyboardTask_Init();
    
    while (1) {
        /* Poll USB HID keyboard buffer (would be populated by USB host callback) */
        /* For now, simple placeholder - actual USB HID parsing in interrupt */
        
        /* Check if any key pressed */
        if (key_code != 0) {
            /* Send keyboard packet to DOS PC */
            Protocol_CreatePacket(&pkt, MSG_KEYBOARD, 0, &key_code, 1);
            Protocol_SendPacket(&pkt);
            
            key_code = 0;
        }
        
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
