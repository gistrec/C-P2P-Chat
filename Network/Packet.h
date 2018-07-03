#ifndef C_P2P_CHAT_PACKET_H
#define C_P2P_CHAT_PACKET_H

#include "../Chat.h"

// Пакет отправляется одному из участников для подключения к сети
// В ответ участник должен отправить пришедшие данные обратно
// И добавить к себе участника в массив участников
#define PACKET_CONNECT 0

// Пакет отправляется участником всем остальным при отключении от сети
// Все убирают участника из массива участников
// #define PACKET_DISCONNECT 1

// Пакет отправляется участнику, если он долго не отвечал на запросы
// При этом участник убирается из массива участников
// Если пришел этот пакет - нужно заново отправить PACKET_CONNECT
// #define PACKET_TIMEOUT 2

// Пакет отправляется участнику для получения всех подключенных клиентов
// В ответ приходит список всех подключенных клиентов
#define PACKET_GET_USERS 3

// Пакет отправляется всем участникам - содержит сообщение
// В ответ нужно отправить PACKET_MESSAGE_ACCEPT
#define PACKET_SEND_MESSAGE 4

// Пакет отправляется участнику в ответ на пакет PACKET_SEND_MESSAGE;
// #define PACKET_MESSAGE_ACCEPT 5;

// Получаем id пакета
int getPacketId(char* data);

// Записываем в буффер пакет
// Возвращаем кол-во записанных байт
int createConnectPacket(char* buf);
int createMessagePacket(char* buf, char* msg, int len_msg);
//int createAcceptPacket(char* buf);
//int createGetUsersPacket(char* buf);
//int createTimeoutPacket(char* buf);

// Передаем в функцию входящих пакет
// В ней будем вызывать соответствующие функции
void receivePacket(char* buf, int size, struct sockaddr_in* address);

#endif //C_P2P_CHAT_PACKET_H
