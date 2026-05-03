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

// Раскладка интерфейса (в строках/колонках терминала)
#define UI_TERM_WIDTH    80
#define UI_TERM_HEIGHT   25
#define UI_CHAT_WIDTH    65       // ширина левой колонки (info + messages + input)
#define UI_CLIENTS_WIDTH 15       // ширина правой колонки со списком клиентов
#define UI_INFO_HEIGHT   5
#define UI_MSG_HEIGHT    17
#define UI_INPUT_HEIGHT  3
#define UI_MSG_HISTORY   16       // сколько последних сообщений хранится
#define UI_MSG_TEXT_LEN  126      // размер буфера для одного сообщения (с таймстемпом)


#define BUFLEN 512     // Максимальный размер буффера
#define MAX_CLIENTS 20 // Максимальное число клиентов
#define DEFAULT_PORT 8888 // Порт
#define MAX_NAME_LENGTH 13 // Максимальная длина имени (в символах, включая '\0')
#define MAX_MSG_CHARS  60  // Максимальная длина сообщения (в символах UTF-8)
#define MAX_MSG_BYTES  (MAX_MSG_CHARS * 4 + 1) // запас для UTF-8 (до 4 байт/символ)


#endif //C_P2P_CHAT_CONFIG_H
