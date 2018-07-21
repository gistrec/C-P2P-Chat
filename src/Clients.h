#ifndef C_P2P_CHAT_CLIENTS_H
#define C_P2P_CHAT_CLIENTS_H

#include "Chat.h"

struct Client {
    int isActive;
    char name[MAX_NAME_LENGTH];
    struct sockaddr_in address;
};
extern struct Client clients[MAX_CLIENTS];


/// Функция нужна для добавления клиента в массив клиентов
void addClient(const struct sockaddr_in* addr, const char* name);

/// Функция нужна для получения указателя на структуру клиента
struct Client* getClient(const struct sockaddr_in* addr);

/// Функция нужна для проверки наличия клиента в массиве клиентов
int existClient(const struct sockaddr_in* addr);

/// Функция нужна для удаления клиента из массива клиентов
void removeClient(struct Client* client);

void getName(const struct Client* client, char* name);

#endif //C_P2P_CHAT_CLIENTS_H
