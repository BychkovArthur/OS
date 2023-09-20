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

    ssize_t ln = read(STDIN_FILENO, input, readed);
    printf("readed в дочернем процессе = %ld\n", readed);
    printf("Считано в дочернем процессе %ld байт\n", ln);
    printf("Считана строка в дочернем процессе %s", input);
    
    return 0;
}