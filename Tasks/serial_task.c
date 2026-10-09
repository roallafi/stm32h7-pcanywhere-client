#include "serial_task.h"
#include "uart_serial.h"
#include "config.h"
#include <string.h>

QueueHandle_t screen_queue = NULL;
QueueHandle_t keyboard_queue = NULL;

int SerialTask_SendHandshake(void) {
    Packet_t pkt;
    uint8_t handshake_msg[] = "PCANYWHERE_STM32_CLIENT";
    
    Protocol_CreatePacket(&pkt, MSG_HANDSHAKE, 0, handshake_msg, strlen((char*)handshake_msg));
    return Protocol_SendPacket(&pkt);
}

int SerialTask_WaitForHandshake(void) {
    Packet_t pkt;
    int timeout = 100000;
    
    while (timeout--) {
        if (Protocol_RecvPacket(&pkt) == 0) {
            if (pkt.type == MSG_ACK) {
                return 0;  /* Connected */
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
    return -1;  /* Timeout */
}

void SerialTask(void *pvParameters) {
    Packet_t pkt, recv_pkt;
    uint8_t screen_buffer[SCREEN_BUFFER_SIZE];
    
    /* Send handshake */
    SerialTask_SendHandshake();
    
    /* Wait for ACK */
    if (SerialTask_WaitForHandshake() == 0) {
        /* Connected - enter receive loop */
        while (1) {
            if (Protocol_RecvPacket(&recv_pkt) == 0) {
                switch (recv_pkt.type) {
                    case MSG_SCREEN:
                        /* Screen data received */
                        if (recv_pkt.seq == 0) {
                            /* First packet - clear buffer */
                            memset(screen_buffer, 0, SCREEN_BUFFER_SIZE);
                        }
                        
                        /* Copy screen chunk */
                        int offset = recv_pkt.seq * PACKET_SIZE;
                        memcpy(&screen_buffer[offset], recv_pkt.data, recv_pkt.length);
                        
                        /* If this is the last screen packet (seq 15), queue for display */
                        if (recv_pkt.seq >= 15) {
                            if (screen_queue != NULL) {
                                xQueueSend(screen_queue, screen_buffer, 0);
                            }
                        }
                        break;
                        
                    case MSG_DISCONNECT:
                        /* Server disconnected */
                        vTaskDelay(pdMS_TO_TICKS(1000));
                        /* Reconnect */
                        SerialTask_SendHandshake();
                        break;
                }
            }
            
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    } else {
        /* Failed to connect */
        while (1) {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}
