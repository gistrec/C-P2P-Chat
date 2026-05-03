#ifndef C_P2P_CHAT_UTILS_H
#define C_P2P_CHAT_UTILS_H

#include <stddef.h>

#include "../Config.h"

struct CliOptions {
    char  name[MAX_NAME_LENGTH];
    int   local_port;
    char* remote_host;   // NULL если не задан (= ждать входящие)
    int   remote_port;
};

/// Парсит аргументы командной строки в структуру.
/// При -h/--help выводит usage и завершает программу с кодом 0.
/// При ошибках разбора печатает сообщение и завершает с кодом 1.
void parseArgs(int argc, char *argv[], struct CliOptions* opts);

/// Копирует строку src в dst (размер dst_size, включая '\0'),
/// никогда не разрезая UTF-8 многобайтовый символ посередине.
void utf8_copy(char* dst, size_t dst_size, const char* src);

#endif //C_P2P_CHAT_UTILS_H
