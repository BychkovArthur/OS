#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/mergeSort.h"
#include "../include/parallelMergeSort.h"
#include "../include/functions.h"

#define SIZE 10
#define MAX_NUMBER 20

// Выделить проге больше памяти

// Указывать через параметр бинарника количество потоков
int main(int argc, char* argv[]) {
    time_t start = time(NULL);
    
    int arr[SIZE];

    for (int i = 0; i < 1; ++i) {

        for (int i = 0; i < SIZE; ++i) {
            arr[i] = rand() % MAX_NUMBER;
            printf("a[i] = %d\n", arr[i]);
        }

        if (argc == 2) {
            parallelMergeSort(8, arr, SIZE);
        } else {
            mergeSort(arr, SIZE);
        }
            if (isArrayStrictlyIncreasing(arr, SIZE)) {
                printf("Проверка №%d УСПЕХ\n", i + 1);
            } else {
                printf("Проверка №%d ОШИБКА\n", i + 1);
            }
                for (int j = 0; j < SIZE; ++j) {
                    printf("%d ", arr[j]);
                }
                printf("\n");

    }
    time_t end = time(NULL);
    printf("Seconds: %ld\n", end - start);
}