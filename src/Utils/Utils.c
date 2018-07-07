#include "Utils.h"

void parseArg(int argc, char *argv[], char** connect_ip, int* connect_port, int* source_port) {
    // Если кол-во аргементов не [1, 3, 4, 6]
    if (argc != 4 && argc != 6 && argc != 3 && argc != 1) {
        escape("Используйте ./chat -connect <ip> <port> [-port <port>]\n");
    }
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-connect") == 0 && (i + 2) <= argc) {
            *connect_ip = argv[i + 1];
            *connect_port = atoi(argv[i + 2]);

            if (*connect_port == 0) {
                escape("Неправильно указан порт\n");
            }
            i += 2;
        }
        if (strcmp(argv[i], "-port") == 0) {
            *source_port = atoi(argv[i + 1]);

            if (*source_port == 0) {
                escape("Неправильно указан порт\n");
            }
            i += 1;
        }
    }
}