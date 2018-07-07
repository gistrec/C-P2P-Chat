#ifndef C_P2P_CHAT_CHAT_H
#define C_P2P_CHAT_CHAT_H

// TODO: убрать лишнее
#include <stdio.h>  // printf()
#include <stdlib.h> // atoi()
#include <string.h> // strcmp()
#include <unistd.h> // read(), close()
#include <arpa/inet.h> // struct sockaddr_in
#include <fcntl.h>  // fcntl()

#include "Network/Packet.h"
#include "Network/Socket.h"

#define BUFLEN 512     // Максимальный размер буффера
#define MAX_CLIENTS 20 // Максимальное число клиентов
#define CHAT_PORT 8888 // Порт

struct client {
    int isActive;
    struct sockaddr_in address;
};
extern struct client clients[MAX_CLIENTS];

/// Функция завершает выполнение программы
/// Выведя при этом строку error
void escape(char* error);

/// Функция нужна для подключения к клиенту
/// Отправляет запрос на подключение, пока не получит ответ
void connectToClient(int sockfd, const struct sockaddr_in* addr);

/// Функция нужна для добавления клиента в массив клиентов
void addClient(struct sockaddr_in* addr);

/// Функция нужна для проверки наличия клиента в массиве клиентов
int existClient(struct sockaddr_in* addr);

/// Функция нужна для удаления клиента из массива клиентов
void removeClient(struct sockaddr_in* addr);

/// Отправляем пакет всем клиентам
void sendPacket(int sockfd, char* buf, int buf_size);


#endif //C_P2P_CHAT_CHAT_H