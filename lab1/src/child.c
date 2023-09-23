#include "stdlib.h"
#include <unistd.h>
#include <stdio.h>
#include "functions.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char **argv)
{
    int childProcessExitStatus = 0;
    int numberOfNumbers;
    int* arrayOfNumbers;
    int qStatus;

    FILE* file = fopen(argv[1], "w");
    if (file == NULL) {
        perror("Can't open file (child)");
        childProcessExitStatus = 1;
        if (write(STDOUT_FILENO, &childProcessExitStatus, sizeof(int)) != sizeof(int)) {
            perror("Can't write childProcessExitStatus (child)");
        }
        exit(1);
    }

    while (1) {

        // Чтение статуса выхода
        if (read(STDIN_FILENO, &qStatus, sizeof(int)) != sizeof(int)) {
            perror("Can't read qStatus (child)");
            childProcessExitStatus = 1;
            if (write(STDOUT_FILENO, &childProcessExitStatus, sizeof(int)) != sizeof(int)) {
                perror("Can't write childProcessExitStatus (child)");
            }
            exit(1);
        }
        if (qStatus) {
            break;
        }
        // Чтение длины массива
        if (read(STDIN_FILENO, &numberOfNumbers, sizeof(int)) != sizeof(int)) {
            perror("Can't read numberOfNumbers (child)");
            childProcessExitStatus = 1;
            if (write(STDOUT_FILENO, &childProcessExitStatus, sizeof(int)) != sizeof(int)) {
                perror("Can't write childProcessExitStatus (child)");
            }
            exit(1);
        }

        arrayOfNumbers = (int *) malloc(sizeof(int) * numberOfNumbers);
        if (arrayOfNumbers == NULL) {
            perror("Unable allocate buffer (child)");
            childProcessExitStatus = 1;
            if (write(STDOUT_FILENO, &childProcessExitStatus, sizeof(int)) != sizeof(int)) {
                perror("Can't write childProcessExitStatus (child)");
            }
            exit(1);
        }

        // Чтение массива чисел
        if (read(STDIN_FILENO, arrayOfNumbers, sizeof(int) * numberOfNumbers) != sizeof(int) * numberOfNumbers) {
            perror("Can't read arrayOfNumbers (child)");
            childProcessExitStatus = 1;
            free(arrayOfNumbers);
            if (write(STDOUT_FILENO, &childProcessExitStatus, sizeof(int)) != sizeof(int)) {
                perror("Can't write childProcessExitStatus (child)");
            }
            exit(1);
        }

        // Получение и запись результата деления
        double resultOfDivision = devide(arrayOfNumbers, numberOfNumbers, &childProcessExitStatus);
        if (resultOfDivision == -1.0 && childProcessExitStatus) {
            if (write(STDOUT_FILENO, &childProcessExitStatus, sizeof(int)) != sizeof(int)) {
                perror("Can't write childProcessExitStatus (child)");
            }
            exit(1);
        }
        if (fprintf(file, "%le\n", resultOfDivision) < 0) {
            perror("Can't write in output file (child)");
            childProcessExitStatus = 1;
            free(arrayOfNumbers);
            if (write(STDOUT_FILENO, &childProcessExitStatus, sizeof(int)) != sizeof(int)) {
                perror("Can't write childProcessExitStatus (child)");
            }
            exit(1);
        }
        
        free(arrayOfNumbers);

        // Передача текущего состояния дочернего процесса в родительский
        if (write(STDOUT_FILENO, &childProcessExitStatus, sizeof(int)) != sizeof(int)) {
            perror("Can't write childProcessExitStatus (child)");
            exit(1);
        }
    }

    if (fclose(file) == EOF) {
        perror("Can't close output file (child)");
        childProcessExitStatus = 1;
        if (write(STDOUT_FILENO, &childProcessExitStatus, sizeof(int)) != sizeof(int)) {
            perror("Can't write childProcessExitStatus (child)");
        }
        exit(1);
    }

    if (write(STDOUT_FILENO, &childProcessExitStatus, sizeof(int)) != sizeof(int)) {
        perror("Can't write childProcessExitStatus (child)");
        exit(1);
    }

    return 0;
}