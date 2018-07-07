#include "Packet.h"

int getPacketId(char* buf) {
    return buf[0];
}

int createConnectRequestPacket(char* buf) {
    buf[0] = PACKET_CONNECT_REQUES;
    return 1;
}


int createConnectAcceptPacket(char* buf) {
    buf[0] = PACKET_CONNECT_ACCEPT;
    return 1;
}

int createMessagePacket(char* buf, int len_msg) {
    for (int i = len_msg-1; i >= 1; i--) {
        buf[i] = buf[i-1];
    }
    buf[0] = PACKET_SEND_MESSAGE;
    return 1 + len_msg;
}