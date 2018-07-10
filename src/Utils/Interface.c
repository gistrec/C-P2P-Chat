#include "Interface.h"

WINDOW* box_info = NULL;
WINDOW* box_client = NULL;
WINDOW* box_messages = NULL;
char messages[16][126] = {{0}};

void initInfoBox() {
    box_info = newwin(5, 65, 0, 0);
    box(box_info, 0, 0);
    wrefresh(box_info);
}

void initClientBox() {
    box_client = newwin(25, 15, 0, 65);
    box(box_client, 0, 0);
    mvwprintw(box_client, 1, 1, "   Клиенты   ");
    mvwprintw(box_client, 2, 0, "├─────────────┤");
    wrefresh(box_client);
}

void initMessageBox() {
    box_messages = newwin(20,65,5,0);
    box(box_messages, 0, 0);
    mvwprintw(box_messages, 17, 0, "├───────────────────────────────────────────────────────────────┤");

    wrefresh(box_messages);
}

void moveCursor() {
    wmove(box_messages, 18, 1);
    wrefresh(box_messages);
}

void updateClientBox() {
    wclear(box_client);
    box(box_client, 0, 0);
    mvwprintw(box_client, 1, 1, "   Клиенты   ");
    mvwprintw(box_client, 2, 0, "├─────────────┤");
    int position = 3;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive == 1) {
            mvwprintw(box_client, position, 1, clients[i].name);
            position++;
        }
    }
    moveCursor();
    wrefresh(box_client);
}

void updateMessageBox() {
    wclear(box_messages);
    box(box_messages, 0, 0);
    mvwprintw(box_messages, 17, 0, "├───────────────────────────────────────────────────────────────┤");
    for (int i = 0; i < 16; i++) {
        mvwprintw(box_messages, i + 1, 1, messages[i]);
    }
    moveCursor();
    wrefresh(box_messages);
}

void m_print() {
    for (int i = 0; i < 16; i++) {
        printf("%d: %s\n", i, messages[i]);
    }
}

void addMessage(const char* msg) {
    printf("%s\n", msg);
    return;
    for (int i = 1; i < 16; i++) {
        memset((char *) &messages[i - 1], ' ', sizeof(char) * 18);
        strcpy((char *) &messages[i - 1], (char *) &messages[i]);
    }
    strcpy((char *) &messages[15], msg);
    updateMessageBox();
}


void updateInfoBox(char* ip, int port, char* name) {
    wclear(box_info);
    box(box_info, 0, 0);
    // Печатаем адрес
    mvwprintw(box_info, 1, 1, "Ваш адрес: ");
    mvwprintw(box_info, 1, 13, ip);
    char str_port[5];
    sprintf((char *) &str_port, "%d", port);
    mvwprintw(box_info, 1, 13 + (int) strlen(ip), ":");
    mvwprintw(box_info, 1, 14 + (int) strlen(ip) , str_port);
    // Печатаем имя
    mvwprintw(box_info, 2, 1, "Ваш ник: ");
    mvwprintw(box_info, 2, 13, name);
    moveCursor();
    wrefresh(box_info);
}

void interface_init() {
    setlocale(LC_ALL,"");
    // Изменяем размер экрана
    printf("\e[8;25;80;t");

    initscr();
    wgetch(box_messages);

    initInfoBox();
    initMessageBox();
    initClientBox();

    moveCursor();
}

void interface_close() {
    endwin();
}