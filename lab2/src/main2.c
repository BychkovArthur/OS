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
pthread_mutex_t mutexQueue;
Task taskQueue[MAX_TASKS_COUNT];
int taskCount = 0;
int currentTaskNumber = 0;
int threadCount;
int tasksComplete = 0;


void executeTask(Task* task) {
    int* array = task->array;
    int* buffer = task->buffer;
    int size = task->size;

    int locationOfSortedArray = threadCount == 4 ? ARRAY : BUFFER;
    mergeSortAlgorithm(array, buffer, size, locationOfSortedArray);
}

void* startThread() {
    while (1) {
        Task task;
        pthread_mutex_lock(&mutexQueue);
        task = taskQueue[currentTaskNumber++];
        if (currentTaskNumber > taskCount) {
            tasksComplete = 1;
        } 
        pthread_mutex_unlock(&mutexQueue);
        if (tasksComplete) {
            break;
        }
        printf("Задание %d начало выполнение\n", currentTaskNumber);
        executeTask(&task);
    }
}


void finishSorting(int** arrays, int** buffers, int forLoopTimes) {
    for (int i = 0; i < forLoopTimes; ++i) {
        int* arr = arrays[i];
        int* buffer = buffers[i];

        // Достаем задания для текущего массива
        // Из массива всех заданий достается задание, которое отвечает за сортировку первой части массива
        // Далее, арифметикой указателей обращаемся к каждой из частей
        Task* task = &taskQueue[i * threadCount];

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
}




int main(int argc, char* argv[]) {

    int arraySize, forLoopTimes;

    if (argc != 4) {
        usage();
        return 1;
    } else {
        arraySize = strToInt(argv[1]);
        forLoopTimes = strToInt(argv[2]);
        threadCount = strToInt(argv[3]);
    }

    // Если я не смогу в массив добавить все задания
    if (forLoopTimes * threadCount > MAX_TASKS_COUNT) {
        perror("Can't create so many tasks");
        return 1;
    }

    int* arrays[forLoopTimes];
    int* buffers[forLoopTimes];
    

    createRandomTasks(arrays, buffers, taskQueue, forLoopTimes,
                      arraySize, threadCount, &taskCount);
    

    printf("tasks: %d\n", taskCount);

    pthread_t threads[threadCount];
    pthread_mutex_init(&mutexQueue, NULL);
    time_t start = time(NULL);

    if (threadCount == 1) {
        // Просто сортируем без потоков
        for (int i = 0; i < forLoopTimes; ++i) {
            int* array = arrays[i];
            int* buffer = buffers[i];
            mergeSortAlgorithm(array, buffer, arraySize, ARRAY);
        }
    } else {
        // Создаем потоки
        for (int i = 0; i < threadCount; ++i) {
            if (pthread_create(&threads[i], NULL, &startThread, NULL)) {
                perror("Thread create error");
            }
        }        
        // Завершение потоков
        for (int i = 0; i < threadCount; ++i) {
            if (pthread_join(threads[i], NULL)) {
                perror("Thread join error");
            }
        }
    }

    // Одним потоком завершаю все сортировки
    // Это тоже можно распихать по разным потокам
    // Эта часть по сути тормозит все...
    printf("Seconds before: %ld\n", time(NULL) - start);
    finishSorting(arrays, buffers, forLoopTimes);
    printf("Seconds after: %ld\n", time(NULL) - start);
    for (int i = 0; i < forLoopTimes; ++i) {
        int* arr = arrays[i];
        if (isArrayStrictlyIncreasing(arr, arraySize)) {
            printf("Проверка №%d УСПЕХ\n", i + 1);
        } else {
            printf("Проверка №%d ОШИБКА\n", i + 1);
        }
    }
    
    
    time_t end = time(NULL);
    // Освободить память
    printf("Seconds: %ld\n", end - start);
    pthread_mutex_destroy(&mutexQueue);
    return 0;
}