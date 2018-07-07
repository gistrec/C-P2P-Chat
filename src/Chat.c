#include "Chat.h"

// NOTICE: Из-за {} вызовется агрегатная инициализация,
// которая для каждого элемента структуры вызовет zero-initialization
struct client clients[MAX_CLIENTS] = {};

void escape(char* error) {
    printf("Error!!!\n");
    printf("%s\n", error);
    exit(1);
}

void connectToClient(int sockfd, const struct sockaddr_in* addr) {
    // Буффер для сообщений
    char buf[10];
    // Длина принятых/отправляемых данных
    int buf_size;

    int connected = 0;

    while (!connected) {
        buf_size = createConnectPacket((char *) &buf);
        send_udp(sockfd, addr, buf, buf_size);
        printf("Запрос на подключение отправлен\n");

        int address_size;
        struct sockaddr_in buf_address;
        // Получаем все данные из сокета
        while ((buf_size = socket_read(sockfd, (char *) &buf, &buf_address, &address_size)) != -1) {
            int packet_id = getPacketId((char *) &buf);
            if (packet_id == PACKET_CONNECT && isEquivalAddr(addr, &buf_address)) {
                printf("Подключение прошло успешно!\n");
                return;
            }
        }
        sleep(5);
        printf("Нет ответа от клиента\n");
    }
}

void addClient(struct sockaddr_in* addr) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive == 0) {
            // Копируем адрес
            memcpy(&(clients[i].address), addr, sizeof(*addr));
            clients[i].isActive = 1;
            return;
        }
    }
}

int existClient(struct sockaddr_in* addr) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive == 1) {
            // Сравниваем ip
            if (clients[i].address.sin_addr.s_addr == addr->sin_addr.s_addr) {
                return 1;
            }else {
                return 0;
            }
        }
    }
}

void removeClient(struct sockaddr_in* addr) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive == 1) {
            // Сравниваем ip
            if (clients[i].address.sin_addr.s_addr == addr->sin_addr.s_addr) {
                clients[i].isActive = 0;
                return;
            }
        }
    }
}

void sendPacket(int sockfd, char* buf, int buf_size) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive == 1) {
            send_udp(sockfd, &(clients[i].address), buf, buf_size);

        }
    }
}
