#include "Packet.h"

int getPacketId(const char* buf) {
    return buf[0];
}

int createConnectRequestPacket(char* buf, const char* name) {
    buf[0] = PACKET_CONNECT_REQUES;
    strcpy(buf + 1, name);
    return 1 + (int) strlen(name);
}


int createConnectAcceptPacket(char* buf, const char* name) {
    buf[0] = PACKET_CONNECT_ACCEPT;
    strcpy(buf + 1, name);
    return 1 + (int) strlen(name);
}

int createMessagePacket(char* buf, int len_msg) {
    for (int i = len_msg-1; i >= 1; i--) {
        buf[i] = buf[i-1];
    }
    buf[0] = PACKET_SEND_MESSAGE;
    return 1 + len_msg;
}