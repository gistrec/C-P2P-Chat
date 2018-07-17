#include "Clients.h"


// NOTICE: Из-за {} вызовется агрегатная инициализация,
// которая для каждого элемента структуры вызовет zero-initialization
struct client clients[MAX_CLIENTS] = {};


void addClient(const struct sockaddr_in* addr, const char* name) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive == 0) {
            // Копируем адрес
            memcpy(&(clients[i].address), addr, sizeof(*addr));
            strcpy((char *) &(clients[i].name), name);
            clients[i].isActive = 1;
            return;
        }
    }
}

int existClient(const struct sockaddr_in* addr) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive == 1) {
            // Сравниваем ip
            if (isEquivalAddr(addr, &(clients[i].address))) {
                return 1;
            }
        }
    }
    return 0;
}

void removeClient(const struct sockaddr_in* addr) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive == 1) {
            // Сравниваем ip
            if (isEquivalAddr(addr, &(clients[i].address))) {
                clients[i].isActive = 0;
                return;
            }
        }
    }
}


void getName(const struct sockaddr_in* addr, char* name) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive == 1) {
            // Сравниваем ip
            if (isEquivalAddr(addr, &(clients[i].address))) {
                strcpy(name, clients[i].name);
            }
        }
    }
}
