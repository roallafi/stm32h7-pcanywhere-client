# STM32H7 PCAnywhere Remote DOS Desktop Client

STM32H7 Embedded Client for DOS remote desktop control via serial port.

## Hardware
- **MCU**: STM32H743 (or similar STM32H7)
- **RTOS**: FreeRTOS
- **Display**: TFT LCD 320x240 RGB565 (40-pin parallel)
- **Input**: USB Standard Keyboard (USB Host)
- **Communication**: Serial UART RS-232 (9600-115200 baud)

## Features
- USB Host HID keyboard support
- Serial UART protocol (compatible with pcanywhere-dos)
- 80x25 DOS text mode rendering
- 4KB screen buffer display (ASCII + attribute bytes)
- Real-time keyboard forwarding

## Project Structure
```
├── Core/
│   ├── Inc/
│   │   ├── main.h
│   │   ├── config.h
│   │   └── ...
│   └── Src/
│       ├── main.c
│       ├── stm32h7xx_it.c
│       └── ...
├── Drivers/
│   ├── UART/
│   │   ├── uart_serial.h
│   │   └── uart_serial.c
│   ├── USB/
│   │   ├── usb_host.h
│   │   └── usb_host.c
│   ├── TFT/
│   │   ├── tft_display.h
│   │   └── tft_display.c
│   └── Font/
│       ├── font_8x16.h
│       └── font_8x16.c
├── Protocol/
│   ├── protocol.h
│   ├── protocol.c
│   └── packet_handler.c
├── Tasks/
│   ├── serial_task.h
│   ├── serial_task.c
│   ├── keyboard_task.h
│   ├── keyboard_task.c
│   ├── display_task.h
│   └── display_task.c
└── README.md
```

## Build Instructions (Keil MDK)
1. Open project in Keil µVision
2. Configure STM32CubeMX settings
3. Build: Project → Build
4. Flash: Debug → Download

## Usage
1. Power on STM32H7 board
2. Connect USB keyboard to USB Host port
3. Connect Serial UART to DOS PC COM port
4. Run pcanywhere-dos on DOS PC
5. Select Server mode on DOS
6. STM32 will auto-connect and display screen on TFT
7. Type using USB keyboard - sends to DOS PC

## Protocol
Same as pcanywhere-dos:
- MSG_HANDSHAKE (0x01)
- MSG_ACK (0x02)
- MSG_SCREEN (0x07)
- MSG_KEYBOARD (0x08)
- MSG_DISCONNECT (0x09)

## Notes
- Screen buffer: 80×25 = 4000 bytes (ASCII + attribute)
- Each character: 8x16 pixels
- Display: 320x240 RGB565
- Baud rate: 9600 (configurable)
