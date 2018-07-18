#include "Chat.h"
#include "Utils/Utils.h"


int main(int argc, char *argv[]) {
    int connect_port = DEFAULT_PORT;
    int source_port = DEFAULT_PORT;
    char* connect_ip = NULL;
    char name[MAX_NAME_LENGTH] = "";

    parseConnectAddress(argc, argv, &connect_ip, &source_port);
    parseSourcePort(argc, argv, &source_port);
    parseName(argc, argv, (char *) &name);

    if (strcmp((char *) &name, "") == 0) {
        escape("Необходимо ввести имя: -name <имя>");
    }


    fflush(stdin);
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

    updateInfoBox((char *) &name, source_ip, source_port);

    // Устанавливаем неблокирующий флаг дискрипторам
    setNonblockFlag(sockfd);
    setNonblockFlag(0);

    // Если задан ip - патыаеся подключиться
    if (connect_ip != NULL) {
        createAddress(connect_ip, connect_port, &buf_address);

        sprintf((char *) &buf_send, "Подключаемся к %s:%d", connect_ip, connect_port);
        addMessage((char *) &buf_send);

        connectToClient(sockfd, &buf_address, (char *) &name);
    }else {
        addMessage("Ждем подключения");
    }
    while (1) {
        // Зачем-то нужно передавать длину адреса. Вообще для определения IPv4/6
        unsigned int address_size = sizeof(local_address);
        // Получаем все данные из сокета
        while ((buf_read_size = socket_read(sockfd, (char *) &buf_read, &buf_address, &address_size)) != -1) {
            // Не принимаем пакеты от себя же
            if (isEquivalAddr(&local_address, &buf_address)) continue;
            int packet_id = getPacketId((char *) &buf_read);

            struct Client* client = getClient(&buf_address);

            // Если пришел пакет от неавторизованного клиента, и пакет не запрос на авторизацию
            if (client == NULL && packet_id != PACKET_CONNECT_REQUES) continue;

            buf_read[buf_read_size] = '\0';
            char* buf_ip = inet_ntoa(buf_address.sin_addr);
            int buf_port = ntohs(buf_address.sin_port);

            switch (packet_id) {
                case PACKET_CONNECT_REQUES:
                    if (existClient(&buf_address)) {
                        //printf("Получили пакет на подключение от подключенного клиента!\n");
                    }else {
                        strcpy((char *) &buf_name, buf_read + 1);
                        addClient(&buf_address, (char *) &buf_name);
                        updateClientBox();
                        sprintf((char *) &buf_send, "Подключился клиент %s [%s:%d]", buf_name, buf_ip, buf_port);
                        addMessage((char *) &buf_send);
                    }
                    buf_send_size = createConnectAcceptPacket((char *) &buf_send, (char *) &name);
                    send_udp(sockfd, &buf_address, (char *) &buf_send, buf_send_size);
                    break;
                case PACKET_CONNECT_ACCEPT:
                    if (!existClient(&buf_address)) {
                        strcpy((char *) &buf_name, buf_read + 1);
                        addClient(&buf_address, (char *) &buf_name);
                        updateClientBox();

                        sprintf((char *) &buf_send, "Подключились к %s", buf_name);
                        addMessage((char *) &buf_send);
                    }
                    break;
                case PACKET_SEND_MESSAGE:
                    getName(client, (char *) &buf_name);
                    sprintf((char *) &buf_send, "%s: %s", buf_name, buf_read + 1);
                    addMessage(buf_send);
                    decreasePingCount(client); // Уменьшаем кол-во
                    break;
                case PACKET_MESSAGE_ACCEPT:
                    resetPingCount(client);
                    break;
                case PACKET_REQUEST_USERS:
                    buf_send_size = createListUsersPacket((char *) &buf_send);
                    send_udp(sockfd, &buf_address, (char *) &buf_send, buf_send_size);
                    break;
                case PACKET_LIST_USERS:
                    buf_send_size = createConnectRequestPacket((char *) &buf_send, (char *) &name);
                    // Загружаем клиентов
                    int count =  buf_read[1];
                    for (int i = 0; i < count; i++) {
                        memcpy(&(buf_address), buf_read + 2, sizeof(struct sockaddr_in));
                        send_udp(sockfd, &buf_address, (char *) &buf_send, buf_send_size);
                    }
                    break;
            }
        }
        // TODO: NEED REFACTORING!!!!!!!!!!!!!!!!
        static int size_input = 0;
        static char buf_input[100] = {0};
        while (readInput((char *) buf_input, &size_input) == 1) {
            sprintf((char *) &buf_send, "Вы: %s", buf_input);

            addMessage((char *) &buf_send);
            // printf("Отправляем всем сообщение: %s\n", buf);
            createMessagePacket((char *) &buf_send, (char *) &buf_input, size_input);
            sendPacket(sockfd, (char *) &buf_send, size_input + 1);
            memset(buf_input, 0, 100);
            size_input = 0;
        }
    }
    close_socket(sockfd);
    interface_close();
    return 0;
}