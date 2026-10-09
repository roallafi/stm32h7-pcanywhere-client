#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <string.h>
#include "config.h"

/* Message Types */
#define MSG_HANDSHAKE   0x01
#define MSG_ACK         0x02
#define MSG_NAK         0x03
#define MSG_SCREEN      0x07
#define MSG_KEYBOARD    0x08
#define MSG_DISCONNECT  0x09
#define MSG_PING        0x0A

/* Packet Structure */
struct Packet {
    uint8_t type;
    uint8_t seq;
    uint16_t length;
    uint8_t data[PACKET_SIZE];
    uint8_t checksum;
};

typedef struct Packet Packet_t;

void Protocol_CreatePacket(Packet_t *pkt, uint8_t type, uint8_t seq,
                          const uint8_t *data, uint16_t len);
uint8_t Protocol_CalculateChecksum(const Packet_t *pkt);
int Protocol_VerifyPacket(const Packet_t *pkt);
int Protocol_SendPacket(const Packet_t *pkt);
int Protocol_RecvPacket(Packet_t *pkt);

#endif /* PROTOCOL_H */
