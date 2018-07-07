#ifndef C_P2P_CHAT_SOCKET_H
#define C_P2P_CHAT_SOCKET_H

#include "../Chat.h"

/// Функция нужна для создания сокета
/// Возвращает число - дискриптор сокета
int create_socket();

/// Функция нужна для закрытия дискриптора сокета
void close_socket(int sockfd);

/// Функция нужна для названчения локального адреса сокету
/// Входные данные - дискриптор сокета, указатель на
void bind_address(int sockfd, struct sockaddr_in *addr, unsigned short port);

/// Фунция нужна для отправки сообщения
void send_udp(int sockfd, const struct sockaddr_in *addr, char *buf, int buf_size);

/// Блокирующая функция для получения данных с сокета
/// Возвращает кол-во байт, которые были получены
int socket_read(int sockfd, char *buf, struct sockaddr_in* addr, unsigned int *addr_len);

/// Установка неблокирующего флага дескриптору
void setNonblockFlag(int descriptor);

/// Одинаковы ли два адреса
int isEquivalAddr(const struct sockaddr_in* first, const struct sockaddr_in* second);

/// Создаем структуру адреса
void createAddress(char* ip, int port, struct sockaddr_in* addr);

#endif //C_P2P_CHAT_SOCKET_H
