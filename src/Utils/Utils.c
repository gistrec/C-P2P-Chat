#include "Utils.h"

void parseConnectAddress(int argc, char *argv[], char** ip, int* port) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-connect") == 0 && (i + 2) <= argc) {
            *ip = argv[i + 1];
            *port = atoi(argv[i + 2]);

            if (*port == 0) {
                escape("Неправильно указан порт подключения\n");
            }
            return;
        }
    }
}

void parseSourcePort(int argc, char *argv[], int* port) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-port") == 0 && (i + 1) <= argc) {
            *port = atoi(argv[i + 1]);

            if (*port == 0) {
                escape("Неправильно указан локальный порт\n");
            }
            return;
        }
    }
}

void parseName(int argc, char *argv[], char* name) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-name") == 0 && (i + 1) <= argc) {
            strcpy(name, argv[i + 1]);
            return;
        }
    }
}