#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // for sleep
#include "../include/mergeSort.h"
#include "../include/parallelMergeSort.h"
#include "../include/functions.h"
#include "../include/structures.h"
#include "../include/definitions.h"

// Когда надо отсортировать большое количество массивов.
// Сделать две структуры МБ
pthread_mutex_t mutexQueue;
Task taskQueue[MAX_TASKS_COUNT];
int taskCount = 0;
int currentTaskNumber = 0;
int threadCount;
int tasksComplete = 0;

// Потоки входят в эту функцию и поочередно забирают задания
void* startThread() {
    while (1) {
        Task task;
        if (pthread_mutex_lock(&mutexQueue) != 0) {
            perror("Mutex lock error");
            return NULL;
        }
        task = taskQueue[currentTaskNumber++];
        if (currentTaskNumber > taskCount) {
            tasksComplete = 1;
        }
        
        if (pthread_mutex_unlock(&mutexQueue) != 0) {
            perror("Mutex unlock error");
            return NULL;
        }
        if (tasksComplete) {
            break;
        }
        printf("Задание %d начало выполнение\n", currentTaskNumber);
        executeTask(&task, threadCount);
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

    // Если я не смогу в массив добавить все задания
    if (NumberOfArrays * (threadCount + 1) > MAX_TASKS_COUNT) {
        perror("Can't create so many tasks");
        return 1;
    }

    int* arrays[NumberOfArrays];
    int* buffers[NumberOfArrays];
    
    // Создание заданий, которые будут выполнять потоки
    createRandomTasks(arrays, buffers, taskQueue, NumberOfArrays,
                      arraySize, threadCount, &taskCount);
    
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