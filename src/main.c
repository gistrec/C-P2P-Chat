#include "Chat.h"
#include "Network/Socket.h"
#include "Network/Packet.h"

int main(int argc, char *argv[]) {
    fflush(stdin);
    // Если кол-во аргементов не 4, или второй аргумент не -connect
    if (argc != 4 || strcmp(argv[1], "-connect") != 0) {
        printf("Используйте ./chat -connect ip port\n");
        return 1;
    }
    char* connect_ip = argv[2];
    int connect_port = atoi(argv[3]);


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
    bind_address(sockfd, &local_address, CHAT_PORT);

    char* ip = inet_ntoa(local_address.sin_addr);
    int port = ntohs(local_address.sin_port);
    printf("Ваш ip и порт: %s:%d\n", ip, port);

    // Устанавливаем неблокирующий флаг дискрипторам
    setNonblockFlag(sockfd);
    setNonblockFlag(0);

    struct sockaddr_in connect_address;
    createAddress(connect_ip, connect_port, &connect_address);

    printf("Подключаемся к %s:%d\n\n", connect_ip, connect_port);
    connectToClient(sockfd, &connect_address);

    while (1) {
        // Зачем-то нужно передавать длину адреса. Вообще для определения IPv4/6
        int address_size;
        // Получаем все данные из сокета
        while ((buf_size = socket_read(sockfd, (char *) &buf, &buf_address, &address_size)) != -1) {
            buf[buf_size] = '\0';
            char* buf_ip = inet_ntoa(buf_address.sin_addr);
            int buf_port = ntohs(buf_address.sin_port);
            printf("Входящий пакет: %s\n", buf);
            printf("От %s:%d\n\n", buf_ip, buf_port);

            int packet_id = getPacketId((char *) &buf);
            // TODO: Добавить остальные пакеты
            switch (packet_id) {
                case PACKET_CONNECT:
                    addClient(&buf_address);
                    printf("Добавили клиента %s:%d\n\n", buf_ip, buf_port);
                    // Отправляем в ответ такой же пакет
                    send_udp(sockfd, &buf_address, (char *) &buf, buf_size);
                    break;
                case PACKET_SEND_MESSAGE:
                    printf("Пришло сообщение: %s\n\n", buf+1);
                    break;
            }
        }
        // Получаем все введенные строки
        while ((buf_size = (int) read(0, &buf, BUFLEN)) != -1) {
            buf[buf_size] = '\0';
            printf("Отправляем всем сообщение: %s\n", buf);
            createMessagePacket((char *) &buf, buf_size);
            sendPacket(sockfd, (char *) &buf, buf_size+1);
        }
        sleep((unsigned int) 1);
    }
    close_socket(sockfd);
    return 0;
}