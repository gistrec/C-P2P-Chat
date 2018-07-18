#include "Clients.h"


// NOTICE: Из-за {} вызовется агрегатная инициализация,
// которая для каждого элемента структуры вызовет zero-initialization
struct Client clients[MAX_CLIENTS] = {0};


void addClient(const struct sockaddr_in* addr, const char* name) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive <= 0) {
            // Копируем адрес
            memcpy(&(clients[i].address), addr, sizeof(struct sockaddr_in));
            strcpy((char *) &(clients[i].name), name);
            clients[i].isActive = PING_SKIP_TO_TIMEOUT;
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

void removeClient(const struct sockaddr_in* addr) {
    struct Client* client = getClient(addr);

    if (client != NULL) {
        client->isActive = 0;
    }
}


void getName(const struct sockaddr_in* addr, char* name) {
    struct Client* client = getClient(addr);

    if (client != NULL) {
        strcpy(name, (char *) &(client->name));
    }else {
        name[0] = '\0';
    }
}