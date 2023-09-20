#include "../include/functions.h"

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
        result /= array[i];
    }  
    return result;
}