#include "stdlib.h"
#include <unistd.h>
#include <stdio.h>
#include "../include/functions.h"


int main(int argc, char **argv)
{
    // pid_t pid = fork();
    pid_t pid = 0;

    if (pid == -1) {
        perror("fork");
        exit(1);
    }
    if (pid == 0) { // parent procces
        printf("Введите числа:\n");

        char *input;
        size_t n = 1024;
        ssize_t readed;
        input = (char *) malloc(n * sizeof(char)); // Сделать free

        if (input == NULL) {
            perror("Unable allocate buffer");
            exit(1);
        }

        if ((readed = getline(&input, &n, stdin)) != -1) { // Считывается перенос тоже
            printf("Readed %ld symbols\n", readed);
            printf("Input string: %s\n", input);
            printf("n equal: %ld\n", n);
            int numberOfNumbers = getNumberOfNumbers(input, readed);
            printf("Количество чисел: %d\n", numberOfNumbers);
            if (numberOfNumbers > 0) {
                int *arrayOfNumbers = malloc(sizeof(int) * numberOfNumbers);
                fillArrayWithNumbers(input, readed, arrayOfNumbers);
                for (int i = 0; i < numberOfNumbers; ++i) {
                    printf("HERE %d\n", arrayOfNumbers[i]);
                }
                printf("%lf", devide(arrayOfNumbers, numberOfNumbers));
                free(arrayOfNumbers);
            } else {
                printf("Вы не ввели числа!\n");
            }

        }

        
        free(input);
    }
    if (pid > 0) { // child procces
        execl("child", "child", NULL);
    }
}