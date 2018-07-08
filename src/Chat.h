#ifndef C_P2P_CHAT_CHAT_H
#define C_P2P_CHAT_CHAT_H

#include <ncursesw/curses.h> // Interface in terminal
#include <locale.h> // setLocale()
#include <stdio.h>  // printf()
#include <stdlib.h> // atoi()
#include <string.h> // strcmp()
#include <unistd.h> // read(), close()
#include <fcntl.h>  // fcntl()
#include <arpa/inet.h> // struct sockaddr_in
#include <sys/ioctl.h> // ioctl()

#include "Config.h"
#include "Network/Packet.h"
#include "Network/Socket.h"
#include "Utils/Interface.h"

struct client {
    int isActive;
    char name[MAX_NAME_LENGTH];
    struct sockaddr_in address;
};
extern struct client clients[MAX_CLIENTS];


/// Функция завершает выполнение программы
/// Выведя при этом строку error
void escape(char* error);

/// Функция нужна для подключения к клиенту
/// Отправляет запрос на подключение, пока не получит ответ
void connectToClient(int sockfd, const struct sockaddr_in* addr, const char* name);

/// Функция нужна для добавления клиента в массив клиентов
void addClient(struct sockaddr_in* addr, const char* name);

/// Функция нужна для проверки наличия клиента в массиве клиентов
int existClient(struct sockaddr_in* addr);

/// Функция нужна для удаления клиента из массива клиентов
void removeClient(struct sockaddr_in* addr);

/// Отправляем пакет всем клиентам
void sendPacket(int sockfd, char* buf, int buf_size);


#endif //C_P2P_CHAT_CHAT_H