#include "stdlib.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "../include/functions.h"


int main(int argc, char **argv)
{
    int pipe1[2];
    int pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    // pid_t pid = 0;

    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid > 0) { // parent procces
        close(pipe1[0]);
        printf("CHILD PID = %d, PARENT_PID = %d\n", pid, getpid());
        printf("Введите числа:\n");

        char *input;
        size_t n = 1024;
        ssize_t readed;
        input = (char *) malloc(n * sizeof(char)); // Сделать free

        if (input == NULL) {
            perror("Unable allocate buffer");
            exit(1);
        }

        if ((readed = getline(&input, &n, stdin)) == -1) { // Считывается перенос тоже
            perror("Read error from parent process");
            exit(1);
        }

        // Перенаправляем STDOUT на PIPE
        if (dup2(pipe1[1], STDOUT_FILENO) == -1) {
            perror("PIZDEC");
            exit(1);    
        }

        // Пишем в PIPE
        if (write(pipe1[1], &readed, sizeof(ssize_t)) == -1 || write(pipe1[1], input, readed) == -1) {
            perror("Write error from parent process");
            exit(1);
        }

        // wait(NULL);
        // free(input);
    }
    if (pid == 0) { // child procces
        close(pipe1[1]);
        dup2(pipe1[0], STDIN_FILENO);
        execl("child", "child", NULL);
    }
}