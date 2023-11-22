#include "../include/functions.h"

#include <stdio.h>
#include <stdlib.h>

#include "../include/definitions.h"

double devide(int* array, int numberOfNumbers, int* childProcessExitStatus) {
    double result = array[0];
    for (int i = 1; i < numberOfNumbers; ++i) {
        if (array[i] == 0) {
            perror("Divizion by zero (child)");
            *childProcessExitStatus = 1;
            free(array);
            return -1.0;
        }
        result /= array[i];
    }

    return result;
}

int readFileName(char* fileName) {
    int isFileNameVaild = OK;
    int cnt = 0;
    char c = getchar();
    while (!(c == '\n' || c == EOF)) {
        if (cnt == NAME_MAX) {
            isFileNameVaild = ERROR;
            // Cleaning buffer
            while (!(c == '\n' || c == EOF)) {
                c = getchar();
            }
            break;
        }
        fileName[cnt++] = c;
        c = getchar();
    }
    if (cnt == 0) {
        isFileNameVaild = ERROR;
    }
    if (isFileNameVaild == OK) {
        fileName[cnt] = '\0';
    }
    return isFileNameVaild;
}

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

void fillArrayWithNumbers(char* stringOfNumbers, int stringLen, int* array) {
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