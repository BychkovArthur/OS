#include "stdlib.h"
#include <unistd.h>
#include <stdio.h>
#include "../include/functions.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char **argv)
{
    // Читаю количество символов (байт)
    ssize_t readed;
    if (read(STDIN_FILENO, &readed, sizeof(ssize_t)) == -1) {
        perror("can't read readed in child");
        exit(1);
    }
    
    // Чтение строки из PIPE
    char *inputString = (char *) malloc(readed * sizeof(char));
    ssize_t charactersReaded;
    if ((charactersReaded = read(STDIN_FILENO, inputString, readed)) == -1) {
        perror("can't read input in child");
        exit(1);
    }

    // Получение количества чисел
    int numberOfNumbers = getNumberOfNumbers(inputString, charactersReaded);
    if (numberOfNumbers <= 0) {
        perror("Invalid input string");
        exit(1);
    }

    // Получение массива с числами
    int *arrayOfNumbers = malloc(sizeof(int) * numberOfNumbers);
    fillArrayWithNumbers(inputString, charactersReaded, arrayOfNumbers);

    double resultOfDivision = devide(arrayOfNumbers, numberOfNumbers);
    FILE* file = fopen(argv[1], "w");
    fprintf(file,"%le", resultOfDivision);

    free(inputString);
    free(arrayOfNumbers);
    return 0;
}