#include "stdlib.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "functions.h"
#include <signal.h>
#include <linux/limits.h>


int main(int argc, char **argv)
{
    // FileName
    ssize_t fileNameLen = 128;
    char* fileName = (char  *) malloc(fileNameLen * sizeof(char));
    printf("Введите название файла:\n");
    fileNameLen = getline(&fileName, &fileNameLen, stdin);
    if (fileNameLen < 1 || fileNameLen >= PATH_MAX) {
        perror("FileName error (parent)");
        exit(1);
    }
    fileNameValidation(fileName);

    //Pipe's
    int pipe1[2];
    int pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe");
        exit(1);
    }

    // Fork
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    }

    if (pid > 0) { // Parent procces
        
        int qStatus = 0;
        char *inputString;
        size_t n = 1024;
        ssize_t charactersReaded;
        int childProcessExitStatus;

        if (close(pipe1[0]) == -1 || close(pipe2[1]) == -1) {
            perror("Can't close file descriptor (parent)");
            exit(1);
        }

        printf("Введите числа или q для выхода:\n");

        // Перенаправляем STDOUT на PIPE1
        if (dup2(pipe1[1], STDOUT_FILENO) == -1) {
            perror("dup2 stdout error (parent)");
            exit(1);    
        }

        while (1) {
            inputString = (char *) malloc(n * sizeof(char));

            if (inputString == NULL) {
                perror("Unable allocate buffer (parent)");
                exit(1);
            }

            if ((charactersReaded = getline(&inputString, &n, stdin)) == -1) { // Считываем входную строку с переносом
                perror("GetLine error (parent)");
                exit(1);
            }

            updateQuitStatus(inputString, charactersReaded, &qStatus);
            if(write(STDOUT_FILENO, &qStatus, sizeof(int)) != sizeof(int)) {
                perror("Can't write qStatus (parent)");
            }

            if (qStatus) {
                free(inputString);
                break;
            }
            
            // Получение количества чисел в строке
            int numberOfNumbers = getNumberOfNumbers(inputString, charactersReaded);
            if (numberOfNumbers <= 0) {
                perror("Invalid inputString (parent)");
                exit(1);
            }

            // Получение массива с числами
            int *arrayOfNumbers = malloc(sizeof(int) * numberOfNumbers);
            if (arrayOfNumbers == NULL) {
                perror("Unable allocate buffer (parent)");
                exit(1);
            }
            fillArrayWithNumbers(inputString, charactersReaded, arrayOfNumbers);
            free(inputString);
            
            // Передаем размер массива и сам массив
            if (write(STDOUT_FILENO, &numberOfNumbers, sizeof(int)) != sizeof(int) || write(STDOUT_FILENO, arrayOfNumbers, sizeof(int) * numberOfNumbers) != sizeof(int) * numberOfNumbers) {
                perror("Can't write (parent)");
                exit(1);
            }

            free(arrayOfNumbers);

            // Проверка статуса дочернего процесса
            if (read(pipe2[0], &childProcessExitStatus, sizeof(int)) != sizeof(int)) {
                perror("Can't read data from child (parent)");
                exit(1);
            }

            if (childProcessExitStatus) {
                perror("Child process error (parent)");
                exit(1);
            }
        }

        if (waitpid(pid, NULL, 0) == -1) {
            perror("WaitPID error (parent)");
            exit(1);
        }

        // Проверка статуса дочернего процесса
        if (read(pipe2[0], &childProcessExitStatus, sizeof(int)) != sizeof(int)) {
            perror("Can't read data from child (parent)");
            exit(1);
        }

        if (childProcessExitStatus) {
            perror("Child process error (parent)");
            exit(1);
        }

        if (close(pipe1[1]) == -1 || close(pipe2[0]) == -1) {
            perror("Can't close file descriptor (parent)");
            exit(1);
        }

    }

    if (pid == 0) { // Child procces

        if (close(pipe1[1]) == -1 || close(pipe2[0]) == -1) {
            perror("Can't close file descriptor (parent)");
            exit(1);
        }

        if (dup2(pipe1[0], STDIN_FILENO) == -1) {
            perror("dup2 stdin error (child)");
            exit(1);
        }

        if (dup2(pipe2[1], STDOUT_FILENO) == -1) {
            perror("dup2 stdout error (child)");
            exit(1);
        }

        if (execl("./build/child_exe", "./build/child_exe", fileName, NULL) == -1) {
            perror("Exec error (parent)");
            exit(1);
        }

    }

    free(fileName);
    return 0;
}