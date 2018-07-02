#include "Packet.h"

int getPacketId(char* buf) {
    return (int) buf[0];
}

int createConnectPacket(char* buf) {
    buf[0] = PACKET_CONNECT;
    return 1;
}

int createMessagePacket(char* buf, char* msg, char* len_msg) {
    buf[0] = PACKET_CONNECT;
    for (int i = 1; i <= len_msg; i++) {
        buf[i] = msg[i - 1];
    }
    return 1 + len_msg;
}

void receivePacket(char* buf, int size, struct sockaddr_in *address) {
    int packet_id = getPacketId(buf);
    // TODO: Добавить остальные пакеты
    switch (packet_id) {
        case PACKET_CONNECT:
            break;
        case PACKET_SEND_MESSAGE:
            // Получаем
            char* ip = inet_ntoa(address.sin_addr);
            int port = ntohs(address.sin_port);

            buf[size] = '\0';

            printf("Message from %s:%d\n", ip, port);
            printf("%s\n", buf);
            break;
    }
}