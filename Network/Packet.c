#include "Packet.h"

int getPacketId(char* buf) {
    return buf[0];
}

int createConnectPacket(char* buf) {
    buf[0] = PACKET_CONNECT;
    return 1;
}

int createMessagePacket(char* buf, int len_msg) {
    for (int i = len_msg-1; i >= 1; i--) {
        buf[i] = buf[i-1];
    }
    buf[0] = PACKET_SEND_MESSAGE;
    return 1 + len_msg;
}