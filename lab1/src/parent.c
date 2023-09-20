#include "stdlib.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "functions.h"
#include <signal.h>


int main(int argc, char **argv)
{
    // Чтение имени файла
    size_t fileNameLen = 128;
    char* fileName = (char *) malloc(fileNameLen * sizeof(char));
    printf("Введите название файла:\n");
    if (getline(&fileName, &fileNameLen, stdin) == -1) {
        perror("FileName read error");
        exit(1);
    }

    fileNameValidation(fileName);

    int pipe1[2];
    int pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid > 0) { // parent procces
        printf("CHILD PID = %d, PARENT_PID = %d\n", pid, getpid());
        close(pipe1[0]);
        close(pipe2[1]);
        printf("Введите числа:\n");

        char *inputString;
        size_t n = 1024;
        ssize_t charactersReaded;
        inputString = (char *) malloc(n * sizeof(char));

        if (inputString == NULL) {
            perror("Unable allocate buffer");
            exit(1);
        }

        if ((charactersReaded = getline(&inputString, &n, stdin)) == -1) { // Считывается перенос тоже
            perror("Read error from parent process");
            kill(pid, SIGKILL); // Если ничего не прочитали в родительском, убиваем дочерний
            exit(1);
        }

        // Перенаправляем STDOUT на PIPE1
        if (dup2(pipe1[1], STDOUT_FILENO) == -1) {
            perror("dup2 stdout error in parent process");
            exit(1);    
        }

        // Перенаправляем STDIN на PIPE2
        if (dup2(pipe2[0], STDIN_FILENO) == -1) {
            perror("dup2 stdin error in parent process");
            exit(1);
        }

        // Пишем в PIPE
        if (write(pipe1[1], &charactersReaded, sizeof(ssize_t)) == -1 || write(pipe1[1], inputString, charactersReaded) == -1) {
            perror("Write error from parent process");
            exit(1);
        }

        waitpid(pid, NULL, 0);
        free(inputString);
    }

    if (pid == 0) { // child procces
        close(pipe1[1]);
        close(pipe2[0]);
        if (dup2(pipe1[0], STDIN_FILENO) == -1) {
            perror("dup2 stdin error in child process");
            exit(1);
        }

        if (dup2(pipe2[1], STDOUT_FILENO) == -1) {
            perror("dup2 stdout error in child process");
            exit(1);
        }
        execl("./build/child_exe", "child_exe", fileName, NULL);
    }

    free(fileName);
    return 0;
}