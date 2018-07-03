#include "Chat.h"

// NOTICE: Из-за {} вызовется агрегатная инициализация,
// которая для каждого элемента структуры вызовет zero-initialization
struct client clients[MAX_CLIENTS] = {};


void addClient(struct sockaddr_in* addr) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive == 0) {
            clients[i].address = *addr;
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

void sendMessage(char* buf, int buf_size) {
    createMessagePacket(buf, buf_size);
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
