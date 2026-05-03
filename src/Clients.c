#include "Clients.h"


// NOTICE: Из-за {} вызовется агрегатная инициализация,
// которая для каждого элемента структуры вызовет zero-initialization
struct Client clients[MAX_CLIENTS] = {0};


void addClient(const struct sockaddr_in* addr, const char* name) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive <= 0) {
            // Копируем адрес
            memcpy(&(clients[i].address), addr, sizeof(struct sockaddr_in));
            strncpy(clients[i].name, name, MAX_NAME_LENGTH - 1);
            clients[i].name[MAX_NAME_LENGTH - 1] = '\0';
            clients[i].isActive = PING_SKIP_TO_TIMEOUT;
            updateClientBox();
            return;
        }
    }
}

struct Client* getClient(const struct sockaddr_in* addr) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive > 0) {
            // Сравниваем ip
            if (isEquivalAddr(addr, &(clients[i].address))) {
                return &(clients[i]);
            }
        }
    }
    return NULL;
}

int existClient(const struct sockaddr_in* addr) {
    return getClient(addr) != NULL;
}

void removeClient(struct Client* client) {
    client->isActive = 0;
    updateClientBox();
}

void getName(const struct Client* client, char* name) {
    strcpy(name, (char *) &(client->name));
}