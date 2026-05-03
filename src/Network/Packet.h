#ifndef C_P2P_CHAT_PACKET_H
#define C_P2P_CHAT_PACKET_H

// Пакет отправляется одному из участников для подключения к сети.
// В ответ участник должен отправить PACKET_CONNECT_ACCEPT и добавить отправителя.
#define PACKET_CONNECT_REQUES '0'
#define PACKET_CONNECT_ACCEPT '1'

#define PACKET_PING           '3'
// Пакет отправляется участнику, если он долго не отвечал на запросы.
// Если пришел этот пакет — нужно заново отправить PACKET_CONNECT_REQUES.
#define PACKET_TIMEOUT        '4'

// Запрос/ответ — список всех подключённых клиентов.
#define PACKET_REQUEST_USERS  '5'
#define PACKET_LIST_USERS     '6'

// Сообщение чата.
#define PACKET_SEND_MESSAGE   '7'

int getPacketId(const char* data);

int createSimplePacket(char type, char* buf);
int createConnectRequestPacket(char* buf, const char* name);
int createConnectAcceptPacket(char* buf, const char* name);
int createMessagePacket(char* buf_send, char* buf_input, int len_msg);
int createListUsersPacket(char* buf);

#endif //C_P2P_CHAT_PACKET_H
