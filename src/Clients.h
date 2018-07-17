#ifndef C_P2P_CHAT_CLIENTS_H
#define C_P2P_CHAT_CLIENTS_H

#include "Chat.h"

struct client {
    int isActive;
    char name[MAX_NAME_LENGTH];
    struct sockaddr_in address;
};
extern struct client clients[MAX_CLIENTS];


/// Функция нужна для добавления клиента в массив клиентов
void addClient(const struct sockaddr_in* addr, const char* name);

/// Функция нужна для проверки наличия клиента в массиве клиентов
int existClient(const struct sockaddr_in* addr);

/// Функция нужна для удаления клиента из массива клиентов
void removeClient(const struct sockaddr_in* addr);

void getName(const struct sockaddr_in* addr, char* name);

#endif //C_P2P_CHAT_CLIENTS_H
