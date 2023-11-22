#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../include/mergeSort.h"
#include "../include/parallelMergeSort.h"
#include "../include/functions.h"
#include "../include/structures.h"
#include "../include/definitions.h"

pthread_mutex_t mutexQueue;
regularTask_t* regularTaskQueue;
finishTask_t* finishTaskQueue;
int threadCount;
int regularTaskCount = 0;
int regularTaskComplete = 0;
int finishTaskCount = 0;
int finishTaskComplete = 0;
int allTasksComplete = 0;

// Потоки входят в эту функцию и поочередно забирают задания
void* startThread() {
    while (1) {
        regularTask_t regularTask;
        finishTask_t finishTask;
        
        if (pthread_mutex_lock(&mutexQueue) != 0) {
            perror("Mutex lock error");
            return NULL;
        }

        if (regularTaskComplete < regularTaskCount) {
            regularTask = regularTaskQueue[regularTaskComplete++];

        } else if (regularTaskComplete ==  regularTaskCount && finishTaskComplete < finishTaskCount) {
            finishTask = finishTaskQueue[finishTaskComplete++];

        } else if (regularTaskComplete ==  regularTaskCount && finishTaskComplete == finishTaskCount) {
            allTasksComplete = 1;
        }
        
        if (pthread_mutex_unlock(&mutexQueue) != 0) {
            perror("Mutex unlock error");
            return NULL;
        }

        if (allTasksComplete) {
            break;
        }

        if (finishTaskComplete == 0) {
            printf("Обычное задание №%d начало выполнение\n", regularTaskComplete);
            executeRegularTask(&regularTask, threadCount);
        } else {
            printf("Завершающее задание №%d начало выполнение\n", finishTaskComplete);
            executeFinishTask(&finishTask, threadCount);
        }
    }
}

int main(int argc, char* argv[]) {

    int arraySize, NumberOfArrays;

    if (argc != 4) {
        usage();
        return 1;
    } else {
        arraySize = strToInt(argv[1]);
        NumberOfArrays = strToInt(argv[2]);
        threadCount = strToInt(argv[3]);
    }

    regularTaskQueue = (regularTask_t*) malloc(sizeof(regularTask_t) * NumberOfArrays * threadCount);
    finishTaskQueue = (finishTask_t*) malloc(sizeof(finishTask_t) * NumberOfArrays);
    if (regularTaskQueue == NULL || finishTaskQueue == NULL) {
        perror("Can't allocate memory for queue's");
        return 1;
    }

    int* arrays[NumberOfArrays];
    int* buffers[NumberOfArrays];
    
    // Создание заданий, которые будут выполнять потоки
    createRandomTasks(arrays, buffers, regularTaskQueue, finishTaskQueue, NumberOfArrays,
                      arraySize, threadCount, &regularTaskCount, &finishTaskCount);
    
    if (pthread_mutex_init(&mutexQueue, NULL) != 0) {
        perror("Mutex init error");
        return 1;
    }
    
    pthread_t threads[threadCount];
    time_t start = time(NULL);

    if (threadCount == 1) {
        // Просто сортируем без потоков
        for (int i = 0; i < NumberOfArrays; ++i) {
            int* array = arrays[i];
            int* buffer = buffers[i];
            mergeSortAlgorithm(array, buffer, arraySize, ARRAY);
        }
    } else {
        // Создаем потоки
        for (int i = 0; i < threadCount; ++i) {
            if (pthread_create(&threads[i], NULL, &startThread, NULL)) {
                perror("Thread create error");
                return 1;
            }
        }
        // Завершение потоков
        for (int i = 0; i < threadCount; ++i) {
            if (pthread_join(threads[i], NULL)) {
                perror("Thread join error");
                return 1;
            }
        }
    }

    if (isArraysStrictlyIncreasing(arrays, NumberOfArrays, arraySize)) {
        printf("Все %d массивов отсортированы верно\n", NumberOfArrays);
    } else {
        printf("Один из массивов отсортирован неверно!\n\n");
    }
    
    time_t end = time(NULL);
    printf("Seconds: %ld\n", end - start);
    if (pthread_mutex_destroy(&mutexQueue) != 0) {
        perror("Mutex destroy error");
        return 1;
    }
    for (int i = 0; i < NumberOfArrays; ++i) {
        free(arrays[i]);
        free(buffers[i]);
    }
    return 0;
}