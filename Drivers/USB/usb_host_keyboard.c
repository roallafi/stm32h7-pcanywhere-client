#include "usb_host_keyboard.h"
#include "uart_serial.h"
#include "config.h"

static HID_KeyboardReport_t keyboard_report;
static uint8_t last_keys[6] = {0};
static uint8_t key_queue[16];
static uint8_t key_queue_head = 0;
static uint8_t key_queue_tail = 0;

/* HID Keyboard to ASCII Mapping */
static const char hid_to_ascii[256] = {
    0, 0, 0, 0, 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
    'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2',
    '3', '4', '5', '6', '7', '8', '9', '0', 13, 27, 8, 9, ' ', '-', '=', '[',
    ']', '\\', 0, ';', '\'', '`', ',', '.', '/', 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    /* ... more keycodes ... */
};

void USB_Keyboard_Init(void) {
    memset(&keyboard_report, 0, sizeof(HID_KeyboardReport_t));
    memset(key_queue, 0, sizeof(key_queue));
    key_queue_head = 0;
    key_queue_tail = 0;
}

int USB_Keyboard_GetKey(uint8_t *key) {
    if (key_queue_head == key_queue_tail) {
        return -1;  /* No keys */
    }
    
    *key = key_queue[key_queue_tail];
    key_queue_tail = (key_queue_tail + 1) % 16;
    return 0;
}

void USB_Keyboard_SetKey(uint8_t key) {
    uint8_t next_head = (key_queue_head + 1) % 16;
    
    if (next_head != key_queue_tail) {
        key_queue[key_queue_head] = key;
        key_queue_head = next_head;
    }
}

void USB_Keyboard_Process(void) {
    int i;
    
    /* Check for new key presses */
    for (i = 0; i < 6; i++) {
        if (keyboard_report.keycode[i] != 0 && keyboard_report.keycode[i] != last_keys[i]) {
            uint8_t ascii = hid_to_ascii[keyboard_report.keycode[i]];
            
            if (ascii != 0) {
                /* Handle shift */
                if (keyboard_report.modifier & (HID_SHIFT_LEFT | HID_SHIFT_RIGHT)) {
                    if (ascii >= 'a' && ascii <= 'z') {
                        ascii -= 32;  /* Uppercase */
                    }
                }
                
                USB_Keyboard_SetKey(ascii);
            }
            
            last_keys[i] = keyboard_report.keycode[i];
        }
    }
}

/* USB Host HID Keyboard Callback */
void USBH_HID_KeyboardCallback(USBH_HandleTypeDef *phost) {
    HID_KeyboardReport_t *report = (HID_KeyboardReport_t *)USBH_HID_GetReport(phost);
    
    if (report != NULL) {
        memcpy(&keyboard_report, report, sizeof(HID_KeyboardReport_t));
        USB_Keyboard_Process();
    }
}
