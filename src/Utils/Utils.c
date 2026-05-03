#include "Utils.h"

#include <getopt.h>

static void printUsage(const char* prog) {
    printf(
        "Usage: %s --name <nick> [options]\n"
        "\n"
        "Required:\n"
        "  -n, --name <nick>           Никнейм (до %d символов)\n"
        "\n"
        "Optional:\n"
        "  -l, --local-port <port>     Локальный порт (default: %d)\n"
        "  -r, --remote-host <ip>      IP узла, к которому подключаемся\n"
        "  -p, --remote-port <port>    Порт узла (default: %d)\n"
        "  -h, --help                  Показать эту справку\n"
        "\n"
        "Example:\n"
        "  %s --name Alice --remote-host 46.180.227.50 --remote-port 8888 --local-port 8080\n",
        prog, MAX_NAME_LENGTH - 1, DEFAULT_PORT, DEFAULT_PORT, prog);
}

static int parsePort(const char* s, const char* flag) {
    char* end = NULL;
    long v = strtol(s, &end, 10);
    if (end == s || *end != '\0' || v <= 0 || v > 65535) {
        fprintf(stderr, "Неправильно указан порт для %s: %s\n", flag, s);
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
                strncpy(opts->name, optarg, MAX_NAME_LENGTH - 1);
                opts->name[MAX_NAME_LENGTH - 1] = '\0';
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
        fprintf(stderr, "Необходимо указать никнейм: --name <nick>\n\n");
        printUsage(argv[0]);
        exit(EXIT_FAILURE);
    }
}
