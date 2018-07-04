#include "Socket.h"

int create_socket() {
    // TODO: проверка на успешность создания
    //       if (socket == -1) ...
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    return sockfd;
}

void close_socket(int sockfd) {
    close(sockfd);
}

void bind_address(int sockfd, struct sockaddr_in *addr, unsigned short port) {
    memset((char *) addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port); // To network byte order
    addr->sin_addr.s_addr = htonl(INADDR_ANY);

    // TODO: проверить на успешность бинда
    int r = bind(sockfd, (struct sockaddr *) addr, sizeof(*addr));
}

void send_udp(int sockfd, const struct sockaddr_in *addr, char *buf, int buf_size) {
    sendto(sockfd, buf, buf_size, 0, (struct sockaddr*) addr, sizeof(*addr));
}

int socket_read(int sockfd, char *buf, struct sockaddr_in* addr, unsigned int *addr_len) {
    // TODO: проверить на recv_len != -1
    // memset((char *) addr, 0, sizeof(*addr));
    int recv_len = (int) recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*) addr, addr_len);
    return recv_len;
}

void createAddress(char* ip, int port, struct sockaddr_in* addr) {
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(ip);
    addr->sin_port = htons(port);
}