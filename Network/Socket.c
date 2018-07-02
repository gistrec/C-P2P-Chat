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

void bind_address(int sockfd, struct sockaddr_in *addr, unsigned int *addr_size, unsigned short port) {
    memset((char *) addr, 0, sizeof(*addr));
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port); // To network byte order
    addr->sin_addr.s_addr = htonl(INADDR_ANY);

    *addr_size = sizeof(*addr);

    // TODO: проверить на успешность бинда
    int r = bind(sockfd, (struct sockaddr *) addr, *addr_size);
}

void send_udp(int sockfd, const struct sockaddr_in *addr, unsigned int addr_len, char *body) {
    unsigned int body_len = (unsigned int) strlen(body);
    sendto(sockfd, body, body_len, 0, (struct sockaddr*) &addr, addr_len);
}

int socket_receive_blocking(int sockfd, char *buf, struct sockaddr_in* addr, unsigned int *addr_len) {
    // TODO: проверить на recv_len != -1
    memset((char *) addr, 0, sizeof(*addr));
    int recv_len = (int) recvfrom(sockfd, buf, BUFLEN, 0, (struct sockaddr*) &addr, addr_len);
    return recv_len;
}

