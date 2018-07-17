#ifndef C_P2P_CHAT_PACKET_H
#define C_P2P_CHAT_PACKET_H

#include "../Chat.h"

// Пакет отправляется одному из участников для подключения к сети
// В ответ участник должен отправить пакет PACKET_CONNECT_ACCEPT
// И добавить к себе участника в массив участников
#define PACKET_CONNECT_REQUES '0'

// Пакет отправляется в ответ на PACKET_CONNECT
#define PACKET_CONNECT_ACCEPT '1'

// Пакет отправляется участником всем остальным при отключении от сети
// Все убирают участника из массива участников
// #define PACKET_DISCONNECT '2'

// Пакет отправляется участнику, если он долго не отвечал на запросы
// При этом участник убирается из массива участников
// Если пришел этот пакет - нужно заново отправить PACKET_CONNECT
// #define PACKET_TIMEOUT '4'

// Пакет отправляется участнику для получения всех подключенных клиентов
// В ответ приходит список всех подключенных клиентов
#define PACKET_REQUEST_USERS '5'
#define PACKET_LIST_USERS '6'

// Пакет отправляется всем участникам - содержит сообщение
// В ответ нужно отправить PACKET_MESSAGE_ACCEPT
#define PACKET_SEND_MESSAGE '7'

// Пакет отправляется участнику в ответ на пакет PACKET_SEND_MESSAGE;
// #define PACKET_MESSAGE_ACCEPT '5';

// Получаем id пакета
int getPacketId(const char* data);

// Записываем в буффер пакет
// Возвращаем кол-во записанных байт
int createConnectRequestPacket(char* buf, const char* name);
int createConnectAcceptPacket(char* buf, const char* name);
int createMessagePacket(char* buf, int len_msg);
//int createAcceptPacket(char* buf);
int createRequestUsersPacket(char* buf);
int createListUsersPacket(char* buf);
//int createTimeoutPacket(char* buf);

#endif //C_P2P_CHAT_PACKET_H
