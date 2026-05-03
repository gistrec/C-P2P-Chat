#include "Chat.h"
#include "Utils/Utils.h"

void escape(const char* error) {
    endwin();   // безопасный no-op, если ncurses ещё не инициализирован
    fprintf(stderr, "Error: %s\n", error);
    exit(EXIT_FAILURE);
}

// Состояние pending connect: одна попытка одновременно — для P2P чата достаточно.
static struct {
    int active;
    int retries_left;
    int next_send_ticks;
    struct sockaddr_in addr;
} pending_connect = {0, 0, 0, {0}};

static void sendConnectRequest(int sockfd, const struct sockaddr_in* addr, const char* name) {
    char buf[1 + MAX_NAME_LENGTH];
    int n = createConnectRequestPacket(buf, name);
    send_udp(sockfd, addr, buf, n);
}

void startConnect(int sockfd, const struct sockaddr_in* addr, const char* name) {
    pending_connect.active = 1;
    pending_connect.addr = *addr;
    pending_connect.retries_left = CONNECT_MAX_RETRIES;
    pending_connect.next_send_ticks = CONNECT_RETRY_TICKS;
    sendConnectRequest(sockfd, addr, name);
}

void tickPendingConnect(int sockfd, const char* name) {
    if (!pending_connect.active) return;
    if (--pending_connect.next_send_ticks > 0) return;

    if (--pending_connect.retries_left <= 0) {
        pending_connect.active = 0;
        addMessage("Connect failed: no response from peer", MSG_SYSTEM);
        return;
    }
    sendConnectRequest(sockfd, &pending_connect.addr, name);
    pending_connect.next_send_ticks = CONNECT_RETRY_TICKS;
}

int matchPendingConnect(const struct sockaddr_in* addr) {
    if (!pending_connect.active) return 0;
    if (!isEquivalAddr(&pending_connect.addr, addr)) return 0;
    pending_connect.active = 0;
    return 1;
}

void sendPacket(int sockfd, const char* buf, int buf_size) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive > 0) {
            send_udp(sockfd, &(clients[i].address), buf, buf_size);
        }
    }
}
