#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../include/functions.h"
#include "../include/definitions.h"

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

void createRandomTasks(int** arrays, int** buffers, Task* taskQueue, int forLoopTimes, int arraySize, int threadCount, int* taskCount)
{
    srand(time(NULL));

    for (int i = 0; i < forLoopTimes; ++i) {
        int* arr = (int*) malloc(sizeof(int) * arraySize);
        int* buffer = (int*) malloc(sizeof(int) * arraySize);
        arrays[i] = arr; // Запоминаем указатель на массив
        buffers[i] = buffer;

        for (int j = 0; j < arraySize; ++j) {
            arr[j] = rand() % MAX_NUMBER;
            buffer[j] = arr[j];
        }

        // Создаем задания по сортировки части массива равной 1/threadCnt от всего массива
        for (int j = 0; j < threadCount - 1; ++j) {
            // Одно задание - сортировка одной небольшой части массива
            Task task = {
                .array = arr + j * (arraySize / threadCount),
                .buffer = buffer + j * (arraySize / threadCount),
                .size = arraySize / threadCount,
            };
            taskQueue[(*taskCount)++] = task;
        }
        Task task = {
                .array = arr + (threadCount - 1) * (arraySize / threadCount),
                .buffer = buffer + (threadCount - 1) * (arraySize / threadCount),
                .size = arraySize - (threadCount - 1) * (arraySize / threadCount),
        };
        taskQueue[(*taskCount)++] = task;
    }
}