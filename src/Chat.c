#include "Chat.h"

void escape(char* error) {
    printf("Error!!!\n");
    printf("%s\n", error);
    exit(EXIT_FAILURE);
}

void connectToClient(int sockfd, const struct sockaddr_in* addr, const char* name) {
    // Буффер для сообщений
    char buf[100];
    // Длина принятых/отправляемых данных
    int buf_size;

    while (1) {
        buf_size = createConnectRequestPacket((char *) &buf, name);
        send_udp(sockfd, addr, buf, buf_size);
        // addMessage("Запрос на подключение отправлен");
        sleep(2);

        struct sockaddr_in buf_address = {};
        unsigned int address_size = sizeof(struct sockaddr_in); // Оказывается нужна инициализация!!!
        // Получаем все данные из сокета
        while ((buf_size = socket_read(sockfd, (char *) &buf, &buf_address, &address_size)) != -1) {
            buf[buf_size] = '\0';
            int packet_id = getPacketId((char *) &buf);
            if (packet_id == PACKET_CONNECT_ACCEPT && isEquivalAddr(addr, &buf_address)) {

                char buf_name[MAX_NAME_LENGTH * 2];
                strcpy((char *) &buf_name, buf + 1);
                addClient(&buf_address, (char *) &buf_name);
                updateClientBox();

                sprintf((char *) &buf, "Подключились к %s", buf_name);
                addMessage((char *) &buf);
                return;
            }
        }
        // addMessage("Нет ответа от клиента");
    }
}

void sendPacket(int sockfd, char* buf, int buf_size) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive == 1) {
            send_udp(sockfd, &(clients[i].address), buf, buf_size);
        }
    }
}
