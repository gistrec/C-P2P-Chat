#include "Chat.h"

#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "Clients.h"
#include "Config.h"
#include "Network/Packet.h"
#include "Network/Socket.h"
#include "Utils/Interface.h"
#include "Utils/Utils.h"

static volatile sig_atomic_t shutdown_requested = 0;

static void onSignal(int sig) {
    (void) sig;
    shutdown_requested = 1;
}

int main(int argc, char *argv[]) {
    struct CliOptions opts;
    parseArgs(argc, argv, &opts);

    char* name = opts.name;
    int   source_port  = opts.local_port;
    char* connect_ip   = opts.remote_host;
    int   connect_port = opts.remote_port;

    signal(SIGINT,  onSignal);
    signal(SIGTERM, onSignal);

    interface_init();

    // Адрес локального сокета
    struct sockaddr_in local_address;
    // Адрес куда/откуда будем отправлять/получать данные
    // Будет перезаписываться в разных функциях
    struct sockaddr_in buf_address;

    // Буффер для сообщений
    char buf_read[BUFLEN] = {0};
    char buf_send[BUFLEN] = {0};
    // Буффер для имени
    char buf_name[MAX_NAME_LENGTH] = {0};

    // Длина принятых/отправляемых данных
    int buf_read_size = 0;
    int buf_send_size = 0;

    // Создаем сокет
    int sockfd = create_socket();

    // Привязываем адрес к сокету
    bind_address(sockfd, &local_address, source_port);

    char* source_ip = inet_ntoa(local_address.sin_addr);

    updateInfoBox(name, source_ip, source_port);

    // Устанавливаем неблокирующий флаг дискрипторам
    setNonblockFlag(sockfd);
    setNonblockFlag(0);

    // Если задан ip - патыаеся подключиться (асинхронно)
    if (connect_ip != NULL) {
        createAddress(connect_ip, connect_port, &buf_address);

        snprintf((char *) &buf_send, BUFLEN, "Connecting to %s:%d", connect_ip, connect_port);
        addMessage((char *) &buf_send, MSG_SYSTEM);

        startConnect(sockfd, &buf_address, name);
    }else {
        addMessage("Waiting for connection. Type /help for commands.", MSG_SYSTEM);
    }
    int timeToSendPing = SEND_PING_PAUSE;
    while (!shutdown_requested) {
        // Зачем-то нужно передавать длину адреса. Вообще для определения IPv4/6
        unsigned int address_size = sizeof(local_address);
        // Получаем все данные из сокета
        while ((buf_read_size = socket_read(sockfd, (char *) &buf_read, &buf_address, &address_size)) != -1) {
            // Не принимаем пакеты от себя же
            if (isEquivalAddr(&local_address, &buf_address)) {
                continue;
            }
            int packet_id = getPacketId((char *) &buf_read);

            struct Client* client = getClient(&buf_address);

            // Если пришел пакет от неавторизованного клиента, и пакет не запрос на авторизацию
            if (client == NULL && packet_id != PACKET_CONNECT_REQUES && packet_id != PACKET_CONNECT_ACCEPT) {
                continue;
            }

            // Обновляем данные для таймаута
            if (client != NULL) {
                if (packet_id != PACKET_PING) {
                    createSimplePacket(PACKET_PING, (char *) &buf_send);
                    send_udp(sockfd, &buf_address, (char *) &buf_send, 1);
                }
            }

            buf_read[buf_read_size] = '\0';
            char* buf_ip = inet_ntoa(buf_address.sin_addr);
            int buf_port = ntohs(buf_address.sin_port);
            switch (packet_id) {
                case PACKET_CONNECT_REQUES:
                    if (buf_read_size < 2) break;
                    if (!existClient(&buf_address)) {
                        utf8_copy(buf_name, MAX_NAME_LENGTH, buf_read + 1);
                        addClient(&buf_address, buf_name);
                        updateClientBox();
                        snprintf((char *) &buf_send, BUFLEN, "Client %s connected [%s:%d]", buf_name, buf_ip, buf_port);
                        addMessage((char *) &buf_send, MSG_SYSTEM);
                    }
                    buf_send_size = createConnectAcceptPacket((char *) &buf_send, name);
                    send_udp(sockfd, &buf_address, (char *) &buf_send, buf_send_size);
                    break;
                case PACKET_CONNECT_ACCEPT:
                    if (buf_read_size < 2) break;
                    if (!existClient(&buf_address)) {
                        utf8_copy(buf_name, MAX_NAME_LENGTH, buf_read + 1);
                        addClient(&buf_address, buf_name);
                        updateClientBox();

                        snprintf((char *) &buf_send, BUFLEN, "Connected to %s", buf_name);
                        addMessage((char *) &buf_send, MSG_SYSTEM);
                    }
                    if (matchPendingConnect(&buf_address)) {
                        // Это ответ на наш исходящий connect — запросим список остальных пиров
                        buf_send_size = createSimplePacket(PACKET_REQUEST_USERS, (char *) &buf_send);
                        send_udp(sockfd, &buf_address, (char *) &buf_send, buf_send_size);
                    }
                    break;
                case PACKET_PING:
                    client->isActive = PING_SKIP_TO_TIMEOUT;
                    break;
                case PACKET_TIMEOUT:
                    startConnect(sockfd, &buf_address, name);
                    break;
                case PACKET_SEND_MESSAGE:
                    if (buf_read_size < 2) break;
                    getName(client, (char *) &buf_name);
                    snprintf((char *) &buf_send, BUFLEN, "%s: %s", buf_name, buf_read + 1);
                    addMessage(buf_send, MSG_PEER);
                    break;
                case PACKET_REQUEST_USERS:
                    buf_send_size = createListUsersPacket((char *) &buf_send);
                    send_udp(sockfd, &buf_address, (char *) &buf_send, buf_send_size);
                    break;
                case PACKET_LIST_USERS: {
                    if (buf_read_size < 2) break;
                    int count = (unsigned char) buf_read[1];
                    int needed = 2 + count * (int) sizeof(struct sockaddr_in);
                    if (needed > buf_read_size) break;  // битый пакет — игнорируем
                    buf_send_size = createConnectRequestPacket((char *) &buf_send, name);
                    for (int i = 0; i < count; i++) {
                        memcpy(&buf_address,
                               buf_read + 2 + i * (int) sizeof(struct sockaddr_in),
                               sizeof(struct sockaddr_in));
                        if (isEquivalAddr(&local_address, &buf_address)) continue;
                        if (existClient(&buf_address)) continue;
                        send_udp(sockfd, &buf_address, (char *) &buf_send, buf_send_size);
                    }
                    break;
                }
            }
        }
        static int size_input = 0;
        static char buf_input[MAX_MSG_BYTES] = {0};
        while (readInput((char *) buf_input, &size_input) == 1) {
            // Слэш-команды
            if (buf_input[0] == '/') {
                if (strcmp(buf_input, "/quit") == 0 || strcmp(buf_input, "/exit") == 0) {
                    shutdown_requested = 1;
                } else if (strcmp(buf_input, "/help") == 0) {
                    addMessage("Commands: /help /who /clear /quit", MSG_SYSTEM);
                } else if (strcmp(buf_input, "/clear") == 0) {
                    clearMessages();
                } else if (strcmp(buf_input, "/who") == 0) {
                    int n = 0;
                    for (int i = 0; i < MAX_CLIENTS; i++) {
                        if (clients[i].isActive > 0) {
                            snprintf((char *) &buf_send, BUFLEN, "  - %s", clients[i].name);
                            addMessage((char *) &buf_send, MSG_SYSTEM);
                            n++;
                        }
                    }
                    if (n == 0) addMessage("No connected peers", MSG_SYSTEM);
                } else {
                    snprintf((char *) &buf_send, BUFLEN, "Unknown command: %s", buf_input);
                    addMessage((char *) &buf_send, MSG_SYSTEM);
                }
            } else {
                snprintf((char *) &buf_send, BUFLEN, "You: %s", buf_input);
                addMessage((char *) &buf_send, MSG_OWN);
                createMessagePacket((char *) &buf_send, (char *) &buf_input, size_input);
                sendPacket(sockfd, (char *) &buf_send, size_input + 1);
            }
            memset(buf_input, 0, MAX_MSG_BYTES);
            size_input = 0;
        }
        // Проверяем активность клиентов
        if (timeToSendPing-- <= 0) {
            // Уменьшаем всем активным клиентам 'active'
            for (int i = 0; i < MAX_CLIENTS; i++) {
                // Если клиент долго не присылал PACKET_PING
                if (clients[i].isActive == 1) {
                    // Счиатем его отключившимся
                    createSimplePacket(PACKET_TIMEOUT, (char *) &buf_send);
                    send_udp(sockfd, &(clients[i].address), (char *) &buf_send, 1);
                    snprintf((char *) &buf_send, BUFLEN, "Client %s timed out", clients[i].name);
                    addMessage((char *) &buf_send, MSG_SYSTEM);
                    removeClient(&clients[i]);
                }else if(clients[i].isActive > 1) {
                    // Отправляем пакет PING
                    clients[i].isActive--;
                    createSimplePacket(PACKET_PING, (char *) &buf_send);
                    send_udp(sockfd, &(clients[i].address), (char *) &buf_send, 1);
                }
            }
            timeToSendPing = SEND_PING_PAUSE;
        }

        // Повторяем pending connect, если есть
        tickPendingConnect(sockfd, name);
    }
    close_socket(sockfd);
    interface_close();
    return 0;
}