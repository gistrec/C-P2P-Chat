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

int createMessagePacket(char* buf_send, char* buf_input, int len_msg) {
    buf_send[0] = PACKET_SEND_MESSAGE;
    strcpy(buf_send + 1, buf_input);
    return 1 + len_msg;
}

int createRequestUsersPacket(char* buf) {
    buf[0] = PACKET_REQUEST_USERS;
    return 1;
}

int createListUsersPacket(char* buf) {
    buf[0] = PACKET_LIST_USERS;
    buf[1] = 0; // Кол-во клиентов

    int pos = 2;
    // Для всех клиентов
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive == 1) {
            buf[1]++;
            memcpy(buf + pos, &(clients->address), sizeof(struct sockaddr_in));
            pos += sizeof(struct sockaddr_in);
            /*for (int j = 0; j < sizeof(struct sockaddr_in); j++) {
                buf[pos++] = ((char*) &clients->address)[j];
            }*/
        }
    }
    return pos;
}