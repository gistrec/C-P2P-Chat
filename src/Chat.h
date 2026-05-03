#ifndef C_P2P_CHAT_CHAT_H
#define C_P2P_CHAT_CHAT_H

#include <netinet/in.h>

/// Завершает выполнение программы, выведя сообщение в stderr.
/// Безопасно вызывать как до, так и после interface_init().
void escape(const char* error);

/// Инициирует неблокирующее подключение к удалённому узлу.
/// Сразу шлёт PACKET_CONNECT_REQUES и помещает попытку в "pending" —
/// дальше main loop через tickPendingConnect() будет повторять, если ответа нет.
void startConnect(int sockfd, const struct sockaddr_in* addr, const char* name);

/// Должна вызываться раз за итерацию main loop. Если есть pending connect
/// и истёк интервал — повторяет запрос или отказывается после исчерпания попыток.
void tickPendingConnect(int sockfd, const char* name);

/// Если есть pending connect к этому адресу — снимает его и возвращает 1.
int matchPendingConnect(const struct sockaddr_in* addr);

/// Отправляем пакет всем клиентам
void sendPacket(int sockfd, const char* buf, int buf_size);

#endif //C_P2P_CHAT_CHAT_H
