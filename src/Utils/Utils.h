#ifndef C_P2P_CHAT_UTILS_H
#define C_P2P_CHAT_UTILS_H

#include "../Chat.h"

/// Парсим аргументы скрипта
/// -connect <ip> <port>   адрес, к которому подключаемся
/// -port <port>   локальный порт, на котором запустится бот
/// -name <name>   имя, под которым будем подключаться
void parseConnectAddress(int argc, char *argv[], char** ip, int* port);
void parseSourcePort(int argc, char *argv[], int* port);
void parseName(int argc, char *argv[], char* name);

#endif //C_P2P_CHAT_UTILS_H
