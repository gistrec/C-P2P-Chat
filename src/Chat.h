#ifndef C_P2P_CHAT_CHAT_H
#define C_P2P_CHAT_CHAT_H

#include <ncursesw/curses.h> // Interface in terminal
#include <locale.h> // setLocale()
#include <stdio.h>  // printf()
#include <stdlib.h> // atoi()
#include <string.h> // strcmp()
#include <unistd.h> // read(), close()
#include <fcntl.h>  // fcntl()
#include <arpa/inet.h> // struct sockaddr_in
#include <sys/ioctl.h> // ioctl()

#include "Config.h"
#include "Clients.h"
#include "Network/Packet.h"
#include "Network/Socket.h"
#include "Utils/Interface.h"

/// Функция завершает выполнение программы
/// Выведя при этом строку error
void escape(const char* error);

/// Инициирует неблокирующее подключение к удалённому узлу.
/// Сразу шлёт PACKET_CONNECT_REQUES и помещает попытку в "pending" —
/// дальше main loop через tickPendingConnect() будет повторять, если ответа нет.
void startConnect(int sockfd, const struct sockaddr_in* addr, const char* name);

/// Должна вызываться раз за итерацию main loop. Если есть pending connect
/// и истёк интервал — повторяет запрос или отказывается после исчерпания попыток.
void tickPendingConnect(int sockfd, const char* name);

/// Если есть pending connect к этому адресу — снимает его и возвращает 1.
/// Иначе 0. Вызывается из обработчика PACKET_CONNECT_ACCEPT.
int  matchPendingConnect(const struct sockaddr_in* addr);

/// Отправляем пакет всем клиентам
void sendPacket(int sockfd, const char* buf, int buf_size);


#endif //C_P2P_CHAT_CHAT_H
