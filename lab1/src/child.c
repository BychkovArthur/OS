#include "stdlib.h"
#include <unistd.h>
#include <stdio.h>
#include "../include/functions.h"

int main(int argc, char **argv)
{
    
    // Читаю количество символов (байт)
    ssize_t readed;
    if (read(STDIN_FILENO, &readed, sizeof(ssize_t)) == -1) {
        perror("can't read readed in child");
        exit(1);
    }
    
    char *input = (char *) malloc(readed * sizeof(char));
    if (read(STDIN_FILENO, input, readed) == -1) {
        perror("can't read input in child");
        exit(1);
    }
    printf("readed в дочернем процессе = %ld\n", readed);
    printf("Считана строка в дочернем процессе %s", input);
    printf("Имя выходного файла: %s\n", argv[1]);

    free(input);
    return 0;
}