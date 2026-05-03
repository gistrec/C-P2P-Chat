#ifndef C_P2P_CHAT_SOCKET_H
#define C_P2P_CHAT_SOCKET_H

#include <netinet/in.h>

int  create_socket(void);
void close_socket(int sockfd);

void bind_address(int sockfd, struct sockaddr_in* addr, int port);
void send_udp(int sockfd, const struct sockaddr_in* addr, const char* buf, int buf_size);
int  socket_read(int sockfd, char* buf, struct sockaddr_in* addr, unsigned int* addr_len);
void setNonblockFlag(int descriptor);

int  isEquivalAddr(const struct sockaddr_in* first, const struct sockaddr_in* second);
void createAddress(const char* ip, int port, struct sockaddr_in* addr);

#endif //C_P2P_CHAT_SOCKET_H
