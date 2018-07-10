#ifndef C_P2P_CHAT_INTERFACE_H
#define C_P2P_CHAT_INTERFACE_H

#include "../Chat.h"

extern WINDOW* box_info;
extern WINDOW* box_client;
extern WINDOW* box_messages;
// Массив с сообщениями
extern char messages[16][126];

void initInfoBox();
void initClientBox();
void initMessageBox();

void interface_init();
void interface_close();

void updateClientBox();
void updateInfoBox(char* ip, int port, char* name);

void addMessage(const char* msg);
void m_print();
#endif //C_P2P_CHAT_INTERFACE_H
