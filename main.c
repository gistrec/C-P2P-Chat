#include "Chat.h"
#include "Network/Socket.h"


int main() {
    fflush(stdin);
    // Адрес локального сокета
    struct sockaddr_in addr_me;
    unsigned int addr_me_size;
    // Адрес куда/откуда будем отправлять/получать данные
    // Будет перезаписываться в разных функциях
    struct sockaddr_in addr_other;
    unsigned int addr_other_size;

    // Буффер для сообщений
    char buf[BUFLEN];
    // Длина принятых/отправляемых данных
    int data_size;

    // Создаем сокет
    int sockfd = create_socket();
    // Привязываем адрес к сокету
    bind_address(sockfd, &addr_me, &addr_me_size, CHAT_PORT);

    // Устанавливаем неблокирующий флаг дискриптору сокета
    int flags = fcntl(sockfd, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(sockfd, F_SETFL, flags);

    // Устаналиваем неблокирующий флаг stdin
    flags = fcntl(0, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(0, F_SETFL, flags);

    while (1) {
        // Получаем все данные из сокета
        while ((data_size = socket_receive_blocking(sockfd, (char *) buf, &addr_other, &addr_other_size)) != -1) {
            // Получаем ip и port
            char* ip = inet_ntoa(addr_other.sin_addr);
            int port = ntohs(addr_other.sin_port);

            buf[data_size] = '\0';

            printf("Received packet from %s:%d\n", ip, port);
            printf("Size data: %d\n", data_size);
            // TODO: Не может ли быть выхода за границы памяти buf?
            printf("Data: %s\n", buf);
        }
        // Получаем все введенные строки
        while ((data_size = (int) read(0, &buf, BUFLEN)) != -1) {
            buf[data_size] = '\0';
            printf("Write text: %s", buf);
        }
        sleep((unsigned int) 1);
    }
    return 0;
}