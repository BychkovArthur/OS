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
    if (isFileNameVaild == OK) {
        fileName[cnt] = '\0';
    }
    return isFileNameVaild;
}