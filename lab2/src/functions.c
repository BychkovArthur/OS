#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../include/functions.h"
#include "../include/definitions.h"
#include "../include/mergeSort.h"

int isArrayStrictlyIncreasing(int* arr, int size) {
    for (int i = 0; i < size - 1; ++i) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}

int isArraysStrictlyIncreasing(int** arrays, int NumberOfArrays, int arraySize) {
    for (int i = 0; i < NumberOfArrays; ++i) {
        int* array = arrays[i];
        if (!isArrayStrictlyIncreasing(array, arraySize)) {
            return 0;
        }
    }
    return 1;
}

void usage() {
    printf("Usage: <Binary> <Array Size> <Number Of Arrays> <Threads Count>\nArray size must be less than 1e6\nThreads Count can be only 2, 4, 6, 8\n");
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

void createRandomTasks(int** arrays, int** buffers, regularTask_t* regularTaskQueue, finishTask_t* finishTaskQueue, int NumberOfArrays, int arraySize, int threadCount, int* regularTaskCount, int* finishTaskCount)
{
    srand(time(NULL));

    for (int i = 0; i < NumberOfArrays; ++i) {
        int* arr = (int*) malloc(sizeof(int) * arraySize);
        int* buffer = (int*) malloc(sizeof(int) * arraySize);
        if (arr == NULL || buffer == NULL) {
            perror("Can't allocate memory for arrays");
            return;
        }
        arrays[i] = arr; // Запоминаем указатель на массив
        buffers[i] = buffer;

        for (int j = 0; j < arraySize; ++j) {
            arr[j] = rand() % MAX_NUMBER;
            buffer[j] = arr[j];
        }

        // Создаем задания по сортировки части массива равной 1/threadCnt от всего массива
        for (int j = 0; j < threadCount - 1; ++j) {
            // Одно задание - сортировка одной небольшой части массива
            regularTask_t regularTask = {
                .array = arr + j * (arraySize / threadCount),
                .buffer = buffer + j * (arraySize / threadCount),
                .size = arraySize / threadCount,
            };
            regularTaskQueue[(*regularTaskCount)++] = regularTask;
        }
        regularTask_t regularTask = {
                .array = arr + (threadCount - 1) * (arraySize / threadCount),
                .buffer = buffer + (threadCount - 1) * (arraySize / threadCount),
                .size = arraySize - (threadCount - 1) * (arraySize / threadCount),
        };
        regularTaskQueue[(*regularTaskCount)++] = regularTask;
    }

    if (threadCount != 1) {
        // Создаем задания для завершения сортировки
        for (int i = 0; i < NumberOfArrays; ++i) {
            // Беру почти отсортированный массиув и буффер для него
            int* arr = arrays[i];
            int* buffer = buffers[i];

            // Достаем задания для текущего массива
            // Из массива всех заданий достается задание, которое отвечает за сортировку первой части массива
            // Далее, арифметикой указателей обращаемся к каждой из частей
            regularTask_t* regularTaskForFunction = &regularTaskQueue[i * threadCount];

            finishTask_t finishTask = {
                    .array = arr,
                    .buffer = buffer,
                    .regularTask = regularTaskForFunction,
            };
            finishTaskQueue[(*finishTaskCount)++] = finishTask;
        }
    }
}

void executeRegularTask(regularTask_t* task, int threadCount) {
    int* array = task->array;
    int* buffer = task->buffer;
    int size = task->size;

    int locationOfSortedArray = threadCount == 4 ? ARRAY : BUFFER;
    mergeSortAlgorithm(array, buffer, size, locationOfSortedArray);
}

void executeFinishTask(finishTask_t* task, int threadCount) {
    int* array = task->array;
    int* buffer = task->buffer;
    regularTask_t* otherTask = task->regularTask;

    finishTask(array, buffer, otherTask, threadCount);
}

// Функция, которая выполняет последние слияния
void finishTask(int* arr, int* buffer, regularTask_t* task, int threadCount) {

    if (threadCount == 2) {
        // В buff лежит отсортированный массив для каждого потока
        merge(task[0].buffer, task[1].buffer, arr, task[0].size, task[1].size);

    } else if (threadCount == 4) {
        // В arr лежит отсортированный массив для каждого потока
        int* res1 = merge(task[0].array, task[1].array, buffer, task[0].size, task[1].size);
        int* res2 = merge(task[2].array, task[3].array, buffer + task[0].size + task[1].size, task[2].size, task[3].size);
        merge(res1, res2, arr, task[0].size + task[1].size, task[2].size + task[3].size);

    } else if (threadCount == 6) {
        // В buff лежит отсортированный массив для каждого потока
        int* res1 = merge(task[0].buffer, task[1].buffer, arr, task[0].size, task[1].size);
        int* res2 = merge(task[2].buffer, task[3].buffer, arr + task[0].size + task[1].size, task[2].size, task[3].size);
        int* res3 = merge(task[4].buffer, task[5].buffer, arr + task[0].size + task[1].size + task[2].size + task[3].size, task[4].size, task[5].size);

        int* res4 = merge(res1, res2, buffer, task[0].size + task[1].size, task[2].size + task[3].size);
        merge(res4, res3, arr, task[0].size + task[1].size + task[2].size + task[3].size, task[4].size + task[5].size);

    } else if (threadCount == 8) {
        // В buff лежит отсортированный массив для каждого потока
        int* res1 = merge(task[0].buffer, task[1].buffer, arr, task[0].size, task[1].size);
        int* res2 = merge(task[2].buffer, task[3].buffer, arr + task[0].size + task[1].size, task[2].size, task[3].size);
        int* res3 = merge(task[4].buffer, task[5].buffer, arr + task[0].size + task[1].size + task[2].size + task[3].size, task[4].size, task[5].size);
        int* res4 = merge(task[6].buffer, task[7].buffer, arr + task[0].size + task[1].size + task[2].size + task[3].size + task[4].size + task[5].size, task[6].size, task[7].size);

        int* res5 = merge(res1, res2, buffer, task[0].size + task[1].size, task[2].size + task[3].size);
        int* res6 = merge(res3, res4, buffer + task[0].size + task[1].size + task[2].size + task[3].size, task[4].size + task[5].size, task[6].size + task[7].size);

        merge(res5, res6, arr, task[0].size + task[1].size + task[2].size + task[3].size, task[4].size + task[5].size + task[6].size + task[7].size);
    }
}