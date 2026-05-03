#ifndef C_P2P_CHAT_CONFIG_H
#define C_P2P_CHAT_CONFIG_H

// Кол-во итераций цикла чтения сокетов/stdin в секунду
#define TICK_PER_SECOND 10
// Кол-во тиков, через которые будет отправлятся PACKET_PING
#define SEND_PING_PAUSE 10
// Кол-во неотвеченных PACKET_PING, через которое клиент считается неактивным
#define PING_SKIP_TO_TIMEOUT 10

// Сколько тиков ждать между повторами connect-запроса (~2 сек)
#define CONNECT_RETRY_TICKS (2 * TICK_PER_SECOND)
// Максимум попыток установить соединение
#define CONNECT_MAX_RETRIES 10


#define BUFLEN 512     // Максимальный размер буффера
#define MAX_CLIENTS 20 // Максимальное число клиентов
#define DEFAULT_PORT 8888 // Порт
#define MAX_NAME_LENGTH 13 // Максимальная длина имени (в символах, включая '\0')
#define MAX_MSG_CHARS  60  // Максимальная длина сообщения (в символах UTF-8)
#define MAX_MSG_BYTES  (MAX_MSG_CHARS * 4 + 1) // запас для UTF-8 (до 4 байт/символ)


#endif //C_P2P_CHAT_CONFIG_H
