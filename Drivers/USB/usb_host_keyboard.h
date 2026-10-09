#ifndef USB_HOST_KEYBOARD_H
#define USB_HOST_KEYBOARD_H

#include "stm32h7xx_hal.h"
#include "FreeRTOS.h"
#include "queue.h"

#define HID_KEYBOARD_REPORT_SIZE    8

/* HID Keyboard Modifier Bits */
#define HID_CTRL_LEFT               0x01
#define HID_SHIFT_LEFT              0x02
#define HID_ALT_LEFT                0x04
#define HID_GUI_LEFT                0x08
#define HID_CTRL_RIGHT              0x10
#define HID_SHIFT_RIGHT             0x20
#define HID_ALT_RIGHT               0x40
#define HID_GUI_RIGHT               0x80

typedef struct {
    uint8_t modifier;
    uint8_t reserved;
    uint8_t keycode[6];
} HID_KeyboardReport_t;

void USB_Keyboard_Init(void);
void USB_Keyboard_Process(void);
int USB_Keyboard_GetKey(uint8_t *key);
void USB_Keyboard_SetKey(uint8_t key);

#endif /* USB_HOST_KEYBOARD_H */
