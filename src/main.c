#include "Chat.h"
#include "Network/Socket.h"
#include "Network/Packet.h"
#include "Utils/Utils.h"
#include "Utils/Interface.h"

int main(int argc, char *argv[]) {
    interface_init();
    return 0;
    fflush(stdin);

    int connect_port = DEFAULT_PORT;
    int source_port = DEFAULT_PORT;
    char* connect_ip = NULL;
    parseArg(argc, argv, &connect_ip, &connect_port, &source_port);


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

    char* ip = inet_ntoa(local_address.sin_addr);
    printf("Ваш ip и порт: %s:%d\n\n", ip, source_port);

    // Устанавливаем неблокирующий флаг дискрипторам
    setNonblockFlag(sockfd);
    setNonblockFlag(0);

    // Если задан ip - патыаеся подключиться
    if (connect_ip != NULL) {
        createAddress(connect_ip, connect_port, &buf_address);

        printf("Подключаемся к %s:%d\n", connect_ip, connect_port);
        connectToClient(sockfd, &buf_address);
    }else {
        printf("Ждем подключения\n");
    }
    while (1) {
        // Зачем-то нужно передавать длину адреса. Вообще для определения IPv4/6
        int address_size;
        // Получаем все данные из сокета
        while ((buf_size = socket_read(sockfd, (char *) &buf, &buf_address, &address_size)) != -1) {
            buf[buf_size] = '\0';
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
                        addClient(&buf_address);
                        printf("Подключился клиент %s:%d\n\n", buf_ip, buf_port);
                    }
                    buf_size = createConnectAcceptPacket((char *) &buf);
                    send_udp(sockfd, &buf_address, (char *) &buf, buf_size);
                    break;
                case PACKET_SEND_MESSAGE:
                    printf("%s:%d => %s\n", buf_ip, buf_port, buf+1);
                    break;
            }
        }
        // Получаем все введенные строки
        while ((buf_size = (int) read(0, &buf, BUFLEN)) != -1) {
            buf[buf_size] = '\0';
            // printf("Отправляем всем сообщение: %s\n", buf);
            createMessagePacket((char *) &buf, buf_size);
            sendPacket(sockfd, (char *) &buf, buf_size+1);
        }
        sleep((unsigned int) 1);
    }
    close_socket(sockfd);
    return 0;
}