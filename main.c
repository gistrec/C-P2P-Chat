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
    char* buf[BUFLEN];
    // Длина принятых/отправляемых данных
    int data_size;

    // Создаем сокет
    int sockfd = create_socket();
    // Привязываем адрес к сокету
    bind_address(sockfd, &addr_me, &addr_me_size, CHAT_PORT);

    // Вызываем блокирующую функцию для получения пакетов
    data_size = socket_receive_blocking(sockfd, (char*) buf, &addr_other, &addr_other_size);

    // Выводим полученные данные
    printf("Received packet from %s:%d\n", inet_ntoa(addr_other.sin_addr), ntohs(addr_other.sin_port));
    printf("Size data: %d\n", data_size);
    // TODO: Не может ли быть выхода за границы памяти buf?
    printf("Data: %s\n", (char*) buf);

    return 0;
}