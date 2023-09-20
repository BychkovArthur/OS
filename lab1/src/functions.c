#include "../include/functions.h"
#include <stdio.h>
#include <stdlib.h>

int getNumberOfNumbers(char* stringOfNumbers, int stringLen) {
    if (stringLen == 0 || (stringLen == 1 && stringOfNumbers[0] == '\n')) {
        return 0;
    }
    int spaceCount = 0;
    for (int i = 0; i < stringLen; ++i) {
        if (stringOfNumbers[i] == ' ') {
            ++spaceCount;
        }
    }
    return spaceCount + 1;
}

void fillArrayWithNumbers(char *stringOfNumbers, int stringLen, int* array) {
    int currentNumber = 0;
    int arrayIndex = 0;
    for (int i = 0; i < stringLen; ++i) {
        if (!(stringOfNumbers[i] == ' ' || stringOfNumbers[i] == '\n')) {
            currentNumber *= 10;
            currentNumber += stringOfNumbers[i] - '0';
        } else {
            array[arrayIndex] = currentNumber;
            ++arrayIndex;
            currentNumber = 0;
        }
    }
    // Если конец строки через EOF, нужно обработать последний элемент
    if (currentNumber) {
        array[arrayIndex] = currentNumber;
    }
}

double devide(int* array, int numberOfNumbers) {
    double result = array[0];
    for (int i = 1; i < numberOfNumbers; ++i) {
        if (array[i] == 0) {
            perror("divizion by zero");
            exit(1);
        }
        result /= array[i];
    }
    
    return result;
}

void fileNameValidation(char* fileName) {
    int i = 0;
    // Удаление переноса строки
    while (fileName[i] != '\0') {
        if (fileName[i] == '\n') {
            fileName[i] = '\0';
            break;
        }
        ++i;
    }
    if (fileName[0] == '\0') {
        perror("Invalid fileName");
        exit(1);
    }
}