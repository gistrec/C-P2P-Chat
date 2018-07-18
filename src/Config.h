#ifndef C_P2P_CHAT_CONFIG_H
#define C_P2P_CHAT_CONFIG_H

#define BUFLEN 512     // Максимальный размер буффера
#define MAX_CLIENTS 20 // Максимальное число клиентов
#define DEFAULT_PORT 8888 // Порт
#define MAX_NAME_LENGTH 13 // Максимальная длина имени

// Сколько нужно ждать пинга от клиента, не считая его отключившимся
#define PING_SKIP_TO_TIMEOUT 5

#endif //C_P2P_CHAT_CONFIG_H
