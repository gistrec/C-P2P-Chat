#ifndef C_P2P_CHAT_CLIENTS_H
#define C_P2P_CHAT_CLIENTS_H

#include <netinet/in.h>

#include "Config.h"

struct Client {
    int isActive;
    char name[MAX_NAME_LENGTH];
    struct sockaddr_in address;
};
extern struct Client clients[MAX_CLIENTS];

void addClient(const struct sockaddr_in* addr, const char* name);
struct Client* getClient(const struct sockaddr_in* addr);
int existClient(const struct sockaddr_in* addr);
void removeClient(struct Client* client);
void getName(const struct Client* client, char* name);

#endif //C_P2P_CHAT_CLIENTS_H
