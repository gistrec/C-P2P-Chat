#ifndef C_P2P_CHAT_UTILS_H
#define C_P2P_CHAT_UTILS_H

#include "../Chat.h"

/// Парсим аргументы скрипта
/// -connect <ip> <port>   адрес, к которому подключаемся
/// -port <port>   локальный порт, на котором запустится бот
void parseArg(int argc, char *argv[], char** connect_ip, int* connect_port, int* source_port);

#endif //C_P2P_CHAT_UTILS_H
