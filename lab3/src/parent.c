#define _GNU_SOURCE
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "../include/definitions.h"
#include "../include/functions.h"

// TODO:
// - Надо сделать, чтобы в независимости от запуска через бинарник или через make все работало одинаково
// - Передача ошибок от ребенка к родителю

int main() {
    char fileName[NAME_MAX + 1];
    if (readFileName(fileName) == ERROR) {
        printf("Invalid file name %s (parent)\n", fileName);
        return 1;
    }

    // Creating general memory
    int sharedMemoryFD = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, S_IRWXU);

    if (sharedMemoryFD == -1) {
        perror("shm_open (parent)");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork (parent)");
        return 1;
    }

    if (pid == 0) {  // Child process
        if (dup2(sharedMemoryFD, STDIN_FILENO) == -1) {
            perror("dup2 stdin (child)");
            return 1;
        }

        if (execl("./build/child_exe", "./build/child_exe", fileName, NULL) == -1) {
            perror("exec error (child)");
            return 1;
        }
    }

    // Parent process
    while (1) {
        char *inputString = NULL;
        size_t n = 0;
        ssize_t charactersReaded = 0;

        printf("Введите числа или q для выхода:\n");

        if ((charactersReaded = getline(&inputString, &n, stdin)) ==
            -1) {  // Считываем входную строку с переносом
            perror("GetLine error (parent)");
            return 1;
        }

        int numberOfNumbers = getNumberOfNumbers(inputString, charactersReaded);
        if (numberOfNumbers <= 0) {
            perror("Invalid inputString (parent)");
            return 1;
        }

        int *arrayOfNumbers = malloc(sizeof(int) * numberOfNumbers);
        if (arrayOfNumbers == NULL) {
            perror("Unable allocate buffer (parent)");
            return 1;
        }

        fillArrayWithNumbers(inputString, charactersReaded, arrayOfNumbers);
        for (int i = 0; i < numberOfNumbers; ++i) {
            printf("%d\n", arrayOfNumbers[i]);
        }

        free(inputString);
        free(arrayOfNumbers);
    }

    // logic

    shm_unlink(SHARED_MEMORY_NAME);
    return 0;
}