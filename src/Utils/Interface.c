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
    mvwprintw(box_client, 1, 1, "   Clients   ");
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
    mvwprintw(box_client, 1, 1, "   Clients   ");
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
    mvwprintw(box_info, 1, 1, "Address: ");
    mvwprintw(box_info, 1, 11, "%s", ip);
    char str_port[8];
    snprintf(str_port, sizeof(str_port), "%d", port);
    mvwprintw(box_info, 1, 11 + (int) strlen(ip), ":");
    mvwprintw(box_info, 1, 12 + (int) strlen(ip), "%s", str_port);
    // Печатаем имя
    mvwprintw(box_info, 2, 1, "Name: ");
    mvwprintw(box_info, 2, 11, "%s", name);

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
    noecho();      // сами рисуем буфер ввода — иначе echo рассинхронен с UTF-8
    cbreak();      // disable line-buffering
    wtimeout(box_input, 1000 / TICK_PER_SECOND);  // wait 100 milliseconds for input
}

// Перерисовать область ввода поверх рамки.
// Внутренняя ширина бокса = 65 - 2 = 63 колонки (cols 1..63).
static void redrawInputArea(const char* buf) {
    // Сначала чистим содержимое пробелами — иначе после удаления символов
    // в правой части могут остаться старые байты.
    wmove(box_input, 1, 1);
    for (int i = 1; i < 64; i++) waddch(box_input, ' ');
    mvwprintw(box_input, 1, 1, "%s", buf);
    mvwaddch(box_input, 1, 64, ACS_VLINE);
    wrefresh(box_input);
}

static int countUtf8Chars(const char* s, int len) {
    int n = 0;
    for (int i = 0; i < len; i++) {
        if (((unsigned char) s[i] & 0xC0) != 0x80) n++;
    }
    return n;
}

int readInput(char* buf, int* size) {
    int symbol = 0;
    int dirty = 0;
    // getch (c cbreak и timeout)
    // ждет 100мс и возвращает ERR если ничего не введено
    while ((symbol = wgetch(box_input)) != ERR) {
        if (symbol == '\n') {
            // Игнорируем Enter на пустом вводе — нечего отправлять
            if (*size == 0) continue;
            redrawInputArea("");
            return 1;
        }else if (symbol == KEY_BACKSPACE || symbol == 127 || symbol == 8) {
            // Backspace/Delete: ncurses возвращает разные коды на разных терминалах
            //   KEY_BACKSPACE — константа ncurses
            //   127 — ASCII DEL (macOS Terminal, большинство xterm)
            //   8   — ASCII BS  (Ctrl-H)
            if (*size > 0) {
                // Снимаем UTF-8 continuation-байты (10xxxxxx) до ведущего
                do {
                    (*size)--;
                    buf[*size] = 0;
                } while (*size > 0 && ((unsigned char) buf[*size - 1] & 0xC0) == 0x80);
                dirty = 1;
            }
        }else if (symbol >= 0 && symbol < 256 && *size < MAX_MSG_BYTES - 1) {
            unsigned char b = (unsigned char) symbol;
            int is_lead = (b & 0xC0) != 0x80;
            // Лимит проверяем только на ведущих байтах: continuation-байты
            // относятся к уже принятому символу — пропускаем безусловно.
            if (is_lead && countUtf8Chars(buf, *size) >= MAX_MSG_CHARS) continue;
            buf[(*size)++] = (char) symbol;
            dirty = 1;
        }
    }
    if (dirty) redrawInputArea(buf);
    return 0;
}

void interface_close() {
    delwin(box_info);
    delwin(box_client);
    delwin(box_messages);
    delwin(box_input);

    endwin();
}