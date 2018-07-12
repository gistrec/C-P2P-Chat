#ifndef C_P2P_CHAT_INTERFACE_H
#define C_P2P_CHAT_INTERFACE_H

#include "../Chat.h"

void interface_init();
void interface_close();

void updateClientBox();
void updateInfoBox(const char* name, const char* ip, int port);

void addMessage(const char* msg);

int readInput(char* buf, int* size);

#endif //C_P2P_CHAT_INTERFACE_H
