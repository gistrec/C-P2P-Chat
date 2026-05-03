#include "Interface.h"
#include "Utils.h"

#include <langinfo.h>

// ncursesw корректно рисует кириллицу и псевдографику только в UTF-8 локали.
// Если системная локаль (LANG/LC_ALL) не UTF-8 — пробуем стандартные fallback'и.
static void ensureUtf8Locale(void) {
    setlocale(LC_ALL, "");
    if (strcmp(nl_langinfo(CODESET), "UTF-8") == 0) return;

    const char* candidates[] = {
        "C.UTF-8", "en_US.UTF-8", "ru_RU.UTF-8", "en_GB.UTF-8", NULL
    };
    for (int i = 0; candidates[i] != NULL; i++) {
        if (setlocale(LC_ALL, candidates[i]) != NULL) return;
    }
}

static WINDOW* box_info = NULL;
static WINDOW* box_client = NULL;
static WINDOW* box_messages = NULL;
static WINDOW* box_input = NULL;

static char messages[16][126] = {{0}};

static void initInfoBox() {
    box_info = newwin(5, 65, 0, 0);
    box(box_info, 0, 0);
    wrefresh(box_info);
}

static void initClientBox() {
    box_client = newwin(25, 15, 0, 65);
    box(box_client, 0, 0);
    mvwprintw(box_client, 1, 1, "   Клиенты   ");
    mvwprintw(box_client, 2, 0, "├─────────────┤");
    wrefresh(box_client);
}

static void initMessageBox() {
    box_messages = newwin(17, 65, 5, 0);
    box(box_messages, 0, 0);
    mvwprintw(box_messages, 16, 0, "│                                                               │");

    wrefresh(box_messages);
}

static void initInputBox() {
    box_input = newwin(3, 65, 22, 0);
    box(box_input, 0, 0);
    mvwprintw(box_input, 0, 0, "├───────────────────────────────────────────────────────────────┤");;

    wrefresh(box_input);
}

void updateClientBox() {
    wclear(box_client);
    box(box_client, 0, 0);
    mvwprintw(box_client, 1, 1, "   Клиенты   ");
    mvwprintw(box_client, 2, 0, "├─────────────┤");
    int position = 3;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].isActive > 0) {
            mvwprintw(box_client, position, 1, "%s", clients[i].name);
            position++;
        }
    }

    wrefresh(box_client);
}

static void updateMessageBox() {
    wclear(box_messages);
    box(box_messages, 0, 0);
    mvwprintw(box_messages, 16, 0, "│                                                               │");
    for (int i = 0; i < 16; i++) {
        mvwprintw(box_messages, i + 1, 1, "%s", messages[i]);
    }

    wrefresh(box_messages);
}

void addMessage(const char* msg) {
    for (int i = 1; i < 16; i++) {
        memcpy(messages[i - 1], messages[i], sizeof(messages[0]));
    }
    utf8_copy(messages[15], sizeof(messages[15]), msg);
    updateMessageBox();
}


void updateInfoBox(const char* name, const char* ip, int port) {
    wclear(box_info);
    box(box_info, 0, 0);
    // Печатаем адрес
    mvwprintw(box_info, 1, 1, "Ваш адрес: ");
    mvwprintw(box_info, 1, 13, "%s", ip);
    char str_port[8];
    snprintf(str_port, sizeof(str_port), "%d", port);
    mvwprintw(box_info, 1, 13 + (int) strlen(ip), ":");
    mvwprintw(box_info, 1, 14 + (int) strlen(ip), "%s", str_port);
    // Печатаем имя
    mvwprintw(box_info, 2, 1, "Ваш ник: ");
    mvwprintw(box_info, 2, 13, "%s", name);

    wrefresh(box_info);
}

void interface_init() {
    ensureUtf8Locale();
    // Изменяем размер экрана
    printf("\e[8;25;80;t");
    fflush(stdout);

    initscr();

    initInfoBox();
    initMessageBox();
    initClientBox();
    initInputBox();

    keypad(box_input, TRUE);
    echo();
    cbreak();      // disable line-buffering
    wtimeout(box_input, 1000 / TICK_PER_SECOND);  // wait 100 milliseconds for input
}

int readInput(char* buf, int* size) {
    int symbol = 0;
    // getch (c cbreak и timeout)
    // ждет 100мс и возвращает ERR если ничего не введено
    while ((symbol = wgetch(box_input)) != ERR) {
        if (symbol == '\n') {
            // Добавляем сообщение
            // Отчищаем буфер
            for (int i = 0; i < *size; i++) {
                mvwprintw(box_input, 1, i + 1, " ");
            }
            return 1;
        }else if (symbol == KEY_BACKSPACE || symbol == 127 || symbol == 8) {
            // Backspace/Delete: ncurses возвращает разные коды на разных терминалах
            //   KEY_BACKSPACE — стандартная константа ncurses
            //   127 — ASCII DEL (macOS Terminal, многие xterm-конфиги)
            //   8   — ASCII BS  (Ctrl-H, старые tty)
            if (*size > 0) {
                // Если последний символ — продолжение UTF-8 многобайтового,
                // снимаем все continuation-байты (10xxxxxx) до ведущего.
                do {
                    (*size)--;
                    buf[*size] = 0;
                } while (*size > 0 && ((unsigned char) buf[*size - 1] & 0xC0) == 0x80);
                mvwprintw(box_input, 1, (*size) + 1, " ");
                wmove(box_input, 1, (*size) + 1);
            }
        }else if (*size < 99) {
            buf[(*size)++] = (char) symbol;
        }
    }
    mvwprintw(box_input, 1, 1, "%s", buf);
    return 0;
}

void interface_close() {
    delwin(box_info);
    delwin(box_client);
    delwin(box_messages);
    delwin(box_input);

    endwin();
}