#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/mergeSort.h"
#include "../include/parallelMergeSort.h"
#include "../include/functions.h"

#define SIZE 1000000
#define MAX_NUMBER 1000000

// Выделить проге больше памяти

int main(int argc, char* argv[]) {
    int arraySize, forLoopTimes, threadCount;
    if (argc != 4) {
        usage();
        return 1;
    } else {
        arraySize = strToInt(argv[1]);
        forLoopTimes = strToInt(argv[2]);
        threadCount = strToInt(argv[3]);
    }
    
    // int arr[arraySize];
    int* arr = malloc(sizeof(int) * arraySize);

    time_t start = time(NULL);

    for (int i = 0; i < forLoopTimes; ++i) {

        for (int j = 0; j < arraySize; ++j) {
            arr[j] = rand() % MAX_NUMBER;
            // printf("a[i] = %d\n", arr[i]);
        }

        if (threadCount != 1) {
            parallelMergeSort(threadCount, arr, arraySize);
        } else {
            mergeSort(arr, arraySize);
        }

        if (isArrayStrictlyIncreasing(arr, arraySize)) {
            printf("Проверка №%d УСПЕХ\n", i + 1);
        } else {
            printf("Проверка №%d ОШИБКА\n", i + 1);
        }
            // for (int j = 0; j < SIZE; ++j) {
            //     printf("%d ", arr[j]);
            // }
            // printf("\n");

    }
    time_t end = time(NULL);
    printf("Seconds: %ld\n", end - start);
    return 0;
}