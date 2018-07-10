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
    char buf[BUFLEN];
    // Длина принятых/отправляемых данных
    int buf_size;

    // Создаем сокет
    int sockfd = create_socket();

    // Привязываем адрес к сокету
    bind_address(sockfd, &local_address, source_port);

    char* source_ip = inet_ntoa(local_address.sin_addr);

    updateInfoBox(source_ip, source_port, (char *) &name);

    // Устанавливаем неблокирующий флаг дискрипторам
    setNonblockFlag(sockfd);
    setNonblockFlag(0);

    // Если задан ip - патыаеся подключиться
    if (connect_ip != NULL) {
        createAddress(connect_ip, connect_port, &buf_address);

        sprintf((char *) &buf, "Подключаемся к %s:%d", connect_ip, connect_port);
        addMessage((char *) &buf);

        connectToClient(sockfd, &buf_address, (char *) &name);
    }else {
        addMessage("Ждем подключения");
    }
    while (1) {
        // Зачем-то нужно передавать длину адреса. Вообще для определения IPv4/6
        unsigned int address_size;
        // Получаем все данные из сокета
        while ((buf_size = socket_read(sockfd, (char *) &buf, &buf_address, &address_size)) != -1) {
            buf[buf_size] = '\0';
            char* buf_name = NULL;
            char* buf_ip = inet_ntoa(buf_address.sin_addr);
            int buf_port = ntohs(buf_address.sin_port);
            // printf("Входящий пакет: %s\n", buf);
            // printf("От %s:%d\n\n", buf_ip, buf_port);

            int packet_id = getPacketId((char *) &buf);
            // TODO: Добавить остальные пакеты
            switch (packet_id) {
                case PACKET_CONNECT_REQUES:
                    if (existClient(&buf_address)) {
                        //printf("Получили пакет на подключение от подключенного клиента!\n");
                    }else {
                        char buf_name[MAX_NAME_LENGTH];
                        strcpy((char *) &buf_name, buf + 1);
                        addClient(&buf_address, (char *) &buf_name);
                        updateClientBox();

                        sprintf((char *) &buf, "Подключился клиент %s [%s:%d]", buf_name, buf_ip, buf_port);
                        addMessage((char *) &buf);
                    }
                    buf_size = createConnectAcceptPacket((char *) &buf, (char *) &name);
                    send_udp(sockfd, &buf_address, (char *) &buf, buf_size);
                    break;
                case PACKET_SEND_MESSAGE:
                    buf_name = getName(&buf_address);
                    char buf2[100];
                    sprintf((char *) &buf2, "%s: %s", buf_name, buf+1);
                    addMessage(buf2);
                    break;
            }
        }
        // Получаем все введенные строки
        while ((buf_size = (int) read(0, &buf, BUFLEN)) != -1) {
            buf[buf_size - 1] = '\0'; // Последний символ - перенос на новую строку
            char buf2[100];
            sprintf((char *) &buf2, "Вы: %s", buf);

            addMessage((char *) &buf2);
            // printf("Отправляем всем сообщение: %s\n", buf);
            createMessagePacket((char *) &buf, buf_size);
            sendPacket(sockfd, (char *) &buf, buf_size+1);
        }
        sleep((unsigned int) 1);
    }
    close_socket(sockfd);
    return 0;
}