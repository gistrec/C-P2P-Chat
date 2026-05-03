// Юнит-тесты на сборку/разбор пакетов.
// Не зависят от ncurses — Packet.c линкуется с Clients.c напрямую.

#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

#include "../src/Clients.h"
#include "../src/Network/Packet.h"

// Stub'ы для функций из Chat.c / Interface.c — иначе при линковке Socket.c/Clients.c
// потребуется ncurses-зависимый код, ненужный в юнит-тестах.
#include <stdio.h>
#include <stdlib.h>
void updateClientBox(void) {}
void escape(const char* error) {
    fprintf(stderr, "escape() called in tests: %s\n", error);
    exit(1);
}

static void test_simple_packet(void) {
    char buf[16] = {0};
    int n = createSimplePacket(PACKET_PING, buf);
    assert(n == 1);
    assert(buf[0] == PACKET_PING);
    assert(getPacketId(buf) == PACKET_PING);
}

static void test_connect_request_packet(void) {
    char buf[64] = {0};
    int n = createConnectRequestPacket(buf, "Alice");
    assert(n == 1 + 5);
    assert(buf[0] == PACKET_CONNECT_REQUES);
    assert(strcmp(buf + 1, "Alice") == 0);
}

static void test_message_packet(void) {
    char buf[64] = {0};
    int n = createMessagePacket(buf, "hi", 2);
    assert(n == 1 + 2);
    assert(buf[0] == PACKET_SEND_MESSAGE);
    assert(buf[1] == 'h' && buf[2] == 'i');
}

static void test_list_users_packet_empty(void) {
    // Чистим глобал clients — гарантируем, что нет активных
    memset(clients, 0, sizeof(clients[0]) * MAX_CLIENTS);

    char buf[256] = {0};
    int n = createListUsersPacket(buf);
    assert(n == 2);
    assert(buf[0] == PACKET_LIST_USERS);
    assert(buf[1] == 0);  // count = 0
}

static void test_list_users_packet_with_clients(void) {
    memset(clients, 0, sizeof(clients[0]) * MAX_CLIENTS);

    // Заполняем 2 разных клиента
    clients[0].isActive = 5;
    clients[0].address.sin_family = AF_INET;
    clients[0].address.sin_port = htons(8001);

    clients[3].isActive = 5;
    clients[3].address.sin_family = AF_INET;
    clients[3].address.sin_port = htons(8002);

    char buf[256] = {0};
    int n = createListUsersPacket(buf);
    assert(buf[0] == PACKET_LIST_USERS);
    assert(buf[1] == 2);  // count = 2
    assert(n == 2 + 2 * (int) sizeof(struct sockaddr_in));

    // Достаём оба адреса и проверяем порты — раньше тут был баг
    // (clients->address вместо clients[i].address — оба раза копировался [0]).
    struct sockaddr_in a, b;
    memcpy(&a, buf + 2, sizeof(a));
    memcpy(&b, buf + 2 + sizeof(struct sockaddr_in), sizeof(b));
    assert(ntohs(a.sin_port) == 8001);
    assert(ntohs(b.sin_port) == 8002);
}

int main(void) {
    test_simple_packet();
    test_connect_request_packet();
    test_message_packet();
    test_list_users_packet_empty();
    test_list_users_packet_with_clients();

    printf("test_packet: all tests passed\n");
    return 0;
}
