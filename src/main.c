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
    char buf_read[BUFLEN];
    char buf_send[BUFLEN];
    // Длина принятых/отправляемых данных
    int buf_read_size;
    int buf_send_size;

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
        unsigned int address_size;
        // Получаем все данные из сокета
        while ((buf_read_size = socket_read(sockfd, (char *) &buf_read, &buf_address, &address_size)) != -1) {
            buf_read[buf_read_size] = '\0';
            char* buf_name = NULL;
            char* buf_ip = inet_ntoa(buf_address.sin_addr);
            int buf_port = ntohs(buf_address.sin_port);
            // printf("Входящий пакет: %s\n", buf);
            // printf("От %s:%d\n\n", buf_ip, buf_port);

            int packet_id = getPacketId((char *) &buf_read);
            // TODO: Добавить остальные пакеты
            switch (packet_id) {
                case PACKET_CONNECT_REQUES:
                    if (existClient(&buf_address)) {
                        //printf("Получили пакет на подключение от подключенного клиента!\n");
                    }else {
                        char buf_name[MAX_NAME_LENGTH];
                        strcpy((char *) &buf_name, buf_read + 1);
                        addClient(&buf_address, (char *) &buf_name);
                        updateClientBox();

                        sprintf((char *) &buf_send, "Подключился клиент %s [%s:%d]", buf_name, buf_ip, buf_port);
                        addMessage((char *) &buf_send);
                    }
                    buf_send_size = createConnectAcceptPacket((char *) &buf_send, (char *) &name);
                    send_udp(sockfd, &buf_address, (char *) &buf_send, buf_send_size);
                    break;
                case PACKET_SEND_MESSAGE:
                    buf_name = getName(&buf_address);
                    sprintf((char *) &buf_send, "%s: %s", buf_name, buf_read + 1);
                    addMessage(buf_send);
                    break;
                case PACKET_REQUEST_USERS:
                    buf_send_size = createListUsersPacket((char *) &buf_send);
                    send_udp(sockfd, &buf_address, (char *) &buf_send, buf_send_size);
                    break;
                case PACKET_LIST_USERS:
                    buf_send_size = createConnectRequestPacket((char *) &buf_send, name);
                    // Загружаем клиентов

                    for (int i = 0; i < buf_read[i]; i++) {
                        for (int j = 0; j < sizeof(struct sockaddr_in); j++) {
                            ((char *) &buf_address)[2 + i * sizeof(struct sockaddr_in) + j] = buf[i];
                        }
                        send_udp(sockfd, &buf_address, (char))
                    }
            }
        }
        // TODO: NEED REFACTORING!!!!!!!!!!!!!!!!
        static int size = 0;
        static char buf_read[100] = {0};
        while (readInput((char *) buf_read, &size) == 1) {
            sprintf((char *) &buf_read, "Вы: %s", buf_read);

            addMessage((char *) &buf_read);
            // printf("Отправляем всем сообщение: %s\n", buf);
            createMessagePacket((char *) &buf_read, size);
            sendPacket(sockfd, (char *) &buf_read, size + 1);
            memset(buf_read, 0, 100);
            size = 0;
        }
    }
    close_socket(sockfd);
    interface_close();
    return 0;
}