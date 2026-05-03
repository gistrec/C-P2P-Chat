#include "Interface.h"

#include <langinfo.h>
#include <locale.h>
#include <ncursesw/curses.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../Clients.h"
#include "../Config.h"
#include "Utils.h"

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

struct StoredMessage {
    char text[UI_MSG_TEXT_LEN];
    int  type;
};
static struct StoredMessage messages[UI_MSG_HISTORY] = {{{0}, 0}};

// Цветовые пары
#define CP_SYSTEM 1
#define CP_OWN    2
#define CP_PEER   3
#define CP_DIM    4

static int has_color = 0;

static void initColors(void) {
    if (!has_colors()) {
        has_color = 0;
        return;
    }
    start_color();
    use_default_colors();   // -1 = цвет терминала по умолчанию
    init_pair(CP_SYSTEM, COLOR_YELLOW, -1);
    init_pair(CP_OWN,    COLOR_GREEN,  -1);
    init_pair(CP_PEER,   COLOR_CYAN,   -1);
    init_pair(CP_DIM,    COLOR_WHITE,  -1);
    has_color = 1;
}

static int colorPairFor(int type) {
    switch (type) {
        case MSG_OWN:    return CP_OWN;
        case MSG_PEER:   return CP_PEER;
        case MSG_SYSTEM:
        default:         return CP_SYSTEM;
    }
}

static void initInfoBox() {
    box_info = newwin(UI_INFO_HEIGHT, UI_CHAT_WIDTH, 0, 0);
    box(box_info, 0, 0);
    wrefresh(box_info);
}

static void initClientBox() {
    box_client = newwin(UI_TERM_HEIGHT, UI_CLIENTS_WIDTH, 0, UI_CHAT_WIDTH);
    box(box_client, 0, 0);
    mvwprintw(box_client, 1, 1, "   Clients   ");
    mvwprintw(box_client, 2, 0, "├─────────────┤");
    wrefresh(box_client);
}

static void initMessageBox() {
    box_messages = newwin(UI_MSG_HEIGHT, UI_CHAT_WIDTH, UI_INFO_HEIGHT, 0);
    box(box_messages, 0, 0);
    mvwprintw(box_messages, UI_MSG_HEIGHT - 1, 0, "│                                                               │");

    wrefresh(box_messages);
}

static void initInputBox() {
    box_input = newwin(UI_INPUT_HEIGHT, UI_CHAT_WIDTH, UI_INFO_HEIGHT + UI_MSG_HEIGHT, 0);
    box(box_input, 0, 0);
    mvwprintw(box_input, 0, 0, "├───────────────────────────────────────────────────────────────┤");
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
    mvwprintw(box_messages, UI_MSG_HEIGHT - 1, 0, "│                                                               │");
    for (int i = 0; i < UI_MSG_HISTORY; i++) {
        if (messages[i].text[0] == '\0') continue;
        int cp = colorPairFor(messages[i].type);
        if (has_color) wattron(box_messages, COLOR_PAIR(cp));
        mvwprintw(box_messages, i + 1, 1, "%s", messages[i].text);
        if (has_color) wattroff(box_messages, COLOR_PAIR(cp));
    }
    wrefresh(box_messages);
}

void addMessage(const char* msg, int type) {
    // Префикс с таймстемпом — [HH:MM] msg
    time_t now = time(NULL);
    struct tm tm_buf;
    struct tm* tm = localtime_r(&now, &tm_buf);
    char with_ts[UI_MSG_TEXT_LEN];
    snprintf(with_ts, sizeof(with_ts), "[%02d:%02d] %s", tm->tm_hour, tm->tm_min, msg);

    for (int i = 1; i < UI_MSG_HISTORY; i++) {
        memcpy(&messages[i - 1], &messages[i], sizeof(messages[0]));
    }
    utf8_copy(messages[UI_MSG_HISTORY - 1].text, sizeof(messages[0].text), with_ts);
    messages[UI_MSG_HISTORY - 1].type = type;
    updateMessageBox();
}

void clearMessages(void) {
    memset(messages, 0, sizeof(messages));
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
    // Просим терминал ресайзнуться под нашу раскладку (DECSLPP/DECSCPP).
    // Многие терминалы это игнорируют — раскладка всё равно рассчитана на UI_TERM_*.
    printf("\e[8;%d;%d;t", UI_TERM_HEIGHT, UI_TERM_WIDTH);
    fflush(stdout);

    initscr();
    initColors();

    initInfoBox();
    initMessageBox();
    initClientBox();
    initInputBox();

    keypad(box_input, TRUE);
    noecho();      // сами рисуем буфер ввода — иначе echo рассинхронен с UTF-8
    cbreak();      // disable line-buffering
    wtimeout(box_input, 1000 / TICK_PER_SECOND);  // wait 100 milliseconds for input
}

static int countUtf8Chars(const char* s, int len) {
    int n = 0;
    for (int i = 0; i < len; i++) {
        if (((unsigned char) s[i] & 0xC0) != 0x80) n++;
    }
    return n;
}

// Перерисовать область ввода поверх рамки.
// Внутренняя ширина бокса = 65 - 2 = 63 колонки (cols 1..63).
static void redrawInputArea(const char* buf, int size) {
    // Сначала чистим содержимое пробелами — иначе после удаления символов
    // в правой части могут остаться старые байты.
    wmove(box_input, 1, 1);
    for (int i = 1; i < 64; i++) waddch(box_input, ' ');
    mvwprintw(box_input, 1, 1, "%s", buf);
    mvwaddch(box_input, 1, 64, ACS_VLINE);

    // Счётчик символов на верхней границе: [NN/60]
    int chars = countUtf8Chars(buf, size);
    char counter[16];
    snprintf(counter, sizeof(counter), "[%d/%d]", chars, MAX_MSG_CHARS);
    int len = (int) strlen(counter);
    int at_limit = chars >= MAX_MSG_CHARS;
    if (at_limit) wattron(box_input, A_BOLD | (has_color ? COLOR_PAIR(CP_SYSTEM) : 0));
    mvwprintw(box_input, 0, 64 - len, "%s", counter);
    if (at_limit) wattroff(box_input, A_BOLD | (has_color ? COLOR_PAIR(CP_SYSTEM) : 0));

    wrefresh(box_input);
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
            redrawInputArea("", 0);
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
    if (dirty) redrawInputArea(buf, *size);
    return 0;
}

void interface_close() {
    delwin(box_info);
    delwin(box_client);
    delwin(box_messages);
    delwin(box_input);

    endwin();
}
