#ifndef C_P2P_CHAT_CHAT_H
#define C_P2P_CHAT_CHAT_H

// TODO: убрать лишнее
#include <stdio.h> //printf
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <time.h>

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


/// Функция нужна для добавления клиента в массив клиентов
void addClient(struct sockaddr_in* addr);

/// Функция нужна для проверки наличия клиента в массиве клиентов
int existClient(struct sockaddr_in* addr);

/// Функция нужна для удаления клиента из массива клиентов
void removeClient(struct sockaddr_in* addr);

/// Отправляем всем клиентам сообщение
void sendMessage(char* buf, int buf_size);


#endif //C_P2P_CHAT_CHAT_H