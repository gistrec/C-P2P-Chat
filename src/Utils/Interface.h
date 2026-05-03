#ifndef C_P2P_CHAT_INTERFACE_H
#define C_P2P_CHAT_INTERFACE_H

#include "../Chat.h"

#define MSG_SYSTEM 0
#define MSG_OWN    1
#define MSG_PEER   2

void interface_init();
void interface_close();

void updateClientBox();
void updateInfoBox(const char* name, const char* ip, int port);

void addMessage(const char* msg, int type);
void clearMessages(void);

int readInput(char* buf, int* size);

#endif //C_P2P_CHAT_INTERFACE_H
