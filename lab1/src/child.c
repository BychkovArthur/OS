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
    // Читаю количество символов (байт)
    ssize_t stringLen;
    if (read(STDIN_FILENO, &stringLen, sizeof(ssize_t)) != sizeof(ssize_t)) {
        perror("Can't read stringLen (child)");
        childProcessExitStatus = 1;
        write(STDOUT_FILENO, &childProcessExitStatus, sizeof(int));
        exit(1);
    }
    
    // Чтение строки из PIPE
    char *inputString = (char *) malloc(stringLen * sizeof(char));
    ssize_t charactersReaded;
    if ((charactersReaded = read(STDIN_FILENO, inputString, stringLen)) != stringLen) {
        perror("Can't read inputString (child)");
        childProcessExitStatus = 1;
        write(STDOUT_FILENO, &childProcessExitStatus, sizeof(int));
        exit(1);
    }

    // Получение количества чисел
    int numberOfNumbers = getNumberOfNumbers(inputString, charactersReaded);
    if (numberOfNumbers <= 0) {
        perror("Invalid inputString (child)");
        childProcessExitStatus = 1;
        write(STDOUT_FILENO, &childProcessExitStatus, sizeof(int));
        exit(1);
    }

    // Получение массива с числами
    int *arrayOfNumbers = malloc(sizeof(int) * numberOfNumbers);
    fillArrayWithNumbers(inputString, charactersReaded, arrayOfNumbers);

    // Получение и запись результата деления
    double resultOfDivision = devide(arrayOfNumbers, numberOfNumbers, &childProcessExitStatus);
    FILE* file = fopen(argv[1], "w");
    fprintf(file,"%le", resultOfDivision);

    free(inputString);
    free(arrayOfNumbers);
    write(STDOUT_FILENO, &childProcessExitStatus, sizeof(int));
    return 0;
}