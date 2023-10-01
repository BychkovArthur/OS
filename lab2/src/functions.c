#include <stdio.h>
#include "../include/functions.h"

int isArrayStrictlyIncreasing(int* arr, int size) {
    for (int i = 0; i < size - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

void usage() {
    printf("Usage: <Binary> <Array Size> <For Loop Times> <Threads Count>\nArray size must be less than 1e6\nThreads Count can be only 2, 4, 6, 8\n");
}

int strToInt(char* str) {
    int result = 0;
    int i = 0;
    while (str[i] != '\0') {
        result *= 10;
        result += str[i] - '0';
        ++i;
    }
    return result;
}