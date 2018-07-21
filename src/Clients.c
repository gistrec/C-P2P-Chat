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

void removeClient(struct Client* client) {
    client->isActive = 0;
    updateClientBox();
}

void resetPingCount(struct Client* client) {
    client->isActive = PING_SKIP_TO_TIMEOUT;
}

int decreasePingCount(struct Client* client) {
    client->isActive--;
    return client->isActive;
}

void timeoutClient(struct Client* client) {
    char buf[5];
    createSimplePacket(PACKET_TIMEOUT, (char *) &buf);
    // Отправить пакет клиенту
    removeClient(client);
}

void getName(const struct Client* client, char* name) {
    strcpy(name, (char *) &(client->name));
}