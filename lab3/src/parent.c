#include <stdio.h>
// #include <limits.h>
#include "../include/definitions.h"
#include "../include/functions.h"

// TODO:
// - Надо сделать, чтобы в независимости от запуска через бинарник или через make все работало одинаково

int main() {
    char fileName[NAME_MAX + 1];
    if (readFileName(fileName) == ERROR) {
        printf("Invalid file name %s\n", fileName);
        return 1;
    }
}