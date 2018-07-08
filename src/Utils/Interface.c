#include "Interface.h"

WINDOW* initInfoBox() {
    WINDOW* info = newwin(5, 65, 0, 0);
    box(info, 0, 0);
    wrefresh(info);
    return info;
}

WINDOW* initClientBox() {
    WINDOW *clients = newwin(25, 15, 0, 65);
    // touchwin(clients);
    box(clients, 0, 0);
    mvwprintw(clients, 1, 1, "   Клиенты   ");
    mvwprintw(clients, 2, 0, "├─────────────┤");
    wrefresh(clients);
    return clients;
}

// TODO: need touchwin(chat)
WINDOW* initMessageBox() {
    WINDOW *chat = newwin(20,65,5,0);
    box(chat, 0, 0);
    wrefresh(chat);
    mvwprintw(chat, 17, 0, "├───────────────────────────────────────────────────────────────┤");

    wrefresh(chat);
    return chat;
}

void interface_init() {
    setlocale(LC_ALL,"");
    // Изменяем размер экрана
    printf("\e[8;25;80;t");

    initscr();

    initInfoBox();
    initClientBox();
    initMessageBox();


    getchar();
    endwin();
}