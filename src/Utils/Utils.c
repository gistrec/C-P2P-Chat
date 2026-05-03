#include "Utils.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void utf8_copy(char* dst, size_t dst_size, const char* src) {
    if (dst_size == 0) return;
    size_t i = 0;
    while (src[i] != '\0' && i + 1 < dst_size) {
        unsigned char c = (unsigned char) src[i];
        // Длина текущего UTF-8 символа по старшим битам ведущего байта
        size_t char_len = 1;
        if      ((c & 0x80) == 0x00) char_len = 1;
        else if ((c & 0xE0) == 0xC0) char_len = 2;
        else if ((c & 0xF0) == 0xE0) char_len = 3;
        else if ((c & 0xF8) == 0xF0) char_len = 4;
        // Не помещается целиком — обрываем здесь
        if (i + char_len >= dst_size) break;
        for (size_t k = 0; k < char_len && src[i] != '\0'; k++, i++) {
            dst[i] = src[i];
        }
    }
    dst[i] = '\0';
}

static void printUsage(const char* prog) {
    printf(
        "Usage: %s --name <nick> [options]\n"
        "\n"
        "Required:\n"
        "  -n, --name <nick>           Nickname (up to %d characters)\n"
        "\n"
        "Optional:\n"
        "  -l, --local-port <port>     Local port (default: %d)\n"
        "  -r, --remote-host <ip>      Peer IP to connect to\n"
        "  -p, --remote-port <port>    Peer port (default: %d)\n"
        "  -h, --help                  Show this help and exit\n"
        "\n"
        "Example:\n"
        "  %s --name Alice --remote-host 46.180.227.50 --remote-port 8888 --local-port 8080\n",
        prog, MAX_NAME_LENGTH - 1, DEFAULT_PORT, DEFAULT_PORT, prog);
}

static int parsePort(const char* s, const char* flag) {
    char* end = NULL;
    long v = strtol(s, &end, 10);
    if (end == s || *end != '\0' || v <= 0 || v > 65535) {
        fprintf(stderr, "Invalid port for %s: %s\n", flag, s);
        exit(EXIT_FAILURE);
    }
    return (int) v;
}

void parseArgs(int argc, char *argv[], struct CliOptions* opts) {
    opts->name[0] = '\0';
    opts->local_port = DEFAULT_PORT;
    opts->remote_host = NULL;
    opts->remote_port = DEFAULT_PORT;

    static struct option long_opts[] = {
        {"name",        required_argument, 0, 'n'},
        {"local-port",  required_argument, 0, 'l'},
        {"remote-host", required_argument, 0, 'r'},
        {"remote-port", required_argument, 0, 'p'},
        {"help",        no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "n:l:r:p:h", long_opts, NULL)) != -1) {
        switch (opt) {
            case 'n':
                utf8_copy(opts->name, MAX_NAME_LENGTH, optarg);
                break;
            case 'l':
                opts->local_port = parsePort(optarg, "--local-port");
                break;
            case 'r':
                opts->remote_host = optarg;
                break;
            case 'p':
                opts->remote_port = parsePort(optarg, "--remote-port");
                break;
            case 'h':
                printUsage(argv[0]);
                exit(EXIT_SUCCESS);
            default:
                printUsage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (opts->name[0] == '\0') {
        fprintf(stderr, "Nickname is required: --name <nick>\n\n");
        printUsage(argv[0]);
        exit(EXIT_FAILURE);
    }
}
