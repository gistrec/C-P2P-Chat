// Юнит-тесты для utf8_copy. Запускается отдельным бинарём, без ncurses.
// Падает на assert при первом несовпадении.

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "../src/Utils/Utils.h"

static void test_ascii_fits(void) {
    char buf[16] = "XXXXXXXXXXXXXXX";
    utf8_copy(buf, sizeof(buf), "hello");
    assert(strcmp(buf, "hello") == 0);
}

static void test_empty_source(void) {
    char buf[16] = "preexisting";
    utf8_copy(buf, sizeof(buf), "");
    assert(buf[0] == '\0');
}

static void test_zero_size_dst(void) {
    char buf[1] = {'X'};
    utf8_copy(buf, 0, "anything");
    // Контракт: на dst_size==0 функция не должна писать
    assert(buf[0] == 'X');
}

static void test_truncates_ascii_at_boundary(void) {
    char buf[4];
    utf8_copy(buf, sizeof(buf), "abcdef");
    // 4 байта = 3 символа + '\0'
    assert(strcmp(buf, "abc") == 0);
}

static void test_truncates_utf8_on_codepoint(void) {
    // "Привет" = 6 кириллических символов = 12 байт.
    // В буфер 7 байт (6 байт данных + '\0') влезает ровно 3 символа = "При".
    char buf[7];
    utf8_copy(buf, sizeof(buf), "Привет");
    assert(strcmp(buf, "При") == 0);
}

static void test_does_not_split_multibyte(void) {
    // Буфер 6 байт = 5 байт под данные. Полный 2-байтовый символ влезает,
    // но если бы мы резали по байтам, в строку попал бы половинчатый байт «П\xD1».
    // Проверяем, что utf8_copy не оставляет битый байт.
    char buf[6];
    utf8_copy(buf, sizeof(buf), "Привет");
    // 5 байт под данные = 2 полных символа ("Пр" = 4 байта) + '\0' на 5-м
    assert(strcmp(buf, "Пр") == 0);
}

static void test_4byte_codepoint(void) {
    // Эмодзи "🙂" = 4 байта. В буфер 4 байта (3 байта данных + '\0') не влезает —
    // должна вернуться пустая строка.
    char buf[4];
    utf8_copy(buf, sizeof(buf), "🙂");
    assert(buf[0] == '\0');

    // А в буфер 5 байт (4 данных + '\0') — влезает.
    char buf2[5];
    utf8_copy(buf2, sizeof(buf2), "🙂");
    assert(strcmp(buf2, "🙂") == 0);
}

int main(void) {
    test_ascii_fits();
    test_empty_source();
    test_zero_size_dst();
    test_truncates_ascii_at_boundary();
    test_truncates_utf8_on_codepoint();
    test_does_not_split_multibyte();
    test_4byte_codepoint();

    printf("test_utf8_copy: all tests passed\n");
    return 0;
}
