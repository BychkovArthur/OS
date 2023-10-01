#include <pthread.h>
#include <stdio.h>
#include "../include/mergeSort.h"
#include "../include/parallelMergeSort.h"
#include "../include/structures.h"



void* parallelMergeSortAlgorithm(void* dataForThread) {
    threadData_t* threadData = (threadData_t*) dataForThread;
    int* array = threadData->arr;
    int size = threadData->size;
    mergeSort(array, size);
}

void parallelMergeSort(int threadCount, int* arr, int size) {
    pthread_t threads[threadCount];
    threadData_t data[threadCount];

    // Первые n-1 поток имею одинаковый размер массива, а последний то, что осталось
    for (int i = 0; i < threadCount - 1; ++i) {
        data[i].arr = arr + i * (size / threadCount);
        data[i].size = size / threadCount;
    }
    data[threadCount - 1].arr = arr + (threadCount - 1) * (size / threadCount);
    data[threadCount - 1].size = size - (threadCount - 1) * (size / threadCount);

    // Создание потоков
    for (int i = 0; i < threadCount; ++i) {
        if (pthread_create(&threads[i], NULL, &parallelMergeSortAlgorithm, &data[i])) {
            perror("Thread create error");
        }
    }

    // Ожидание завершения основным потоков созданных
    for (int i = 0; i < threadCount; ++i) {
        if (pthread_join(threads[i], NULL)) {
            perror("Thread join error");
        }
    }

    int buffer[size];
    
    // Везде просто прибавляю сдвиг, чтобы писать в нужное место массива
    if (threadCount == 2) {
        merge(data[0].arr, data[1].arr, buffer, data[0].size, data[1].size);

    } else if (threadCount == 4) {
        int* res1 = merge(data[0].arr, data[1].arr, buffer, data[0].size, data[1].size);
        int* res2 = merge(data[2].arr, data[3].arr, buffer + data[0].size + data[1].size, data[2].size, data[3].size);
        merge(res1, res2, arr, data[0].size + data[1].size, data[2].size + data[3].size);

    } else if (threadCount == 6) {
        int* res1 = merge(data[0].arr, data[1].arr, buffer, data[0].size, data[1].size);
        int* res2 = merge(data[2].arr, data[3].arr, buffer + data[0].size + data[1].size, data[2].size, data[3].size);
        int* res3 = merge(data[4].arr, data[5].arr, buffer + data[0].size + data[1].size + data[2].size + data[3].size, data[4].size, data[5].size);

        int* res4 = merge(res1, res2, arr, data[0].size + data[1].size, data[2].size + data[3].size);
        merge(res4, res3, buffer, data[0].size + data[1].size + data[2].size + data[3].size, data[4].size + data[5].size);

    } else if (threadCount == 8) {
        int* res1 = merge(data[0].arr, data[1].arr, buffer, data[0].size, data[1].size);
        int* res2 = merge(data[2].arr, data[3].arr, buffer + data[0].size + data[1].size, data[2].size, data[3].size);
        int* res3 = merge(data[4].arr, data[5].arr, buffer + data[0].size + data[1].size + data[2].size + data[3].size, data[4].size, data[5].size);
        int* res4 = merge(data[6].arr, data[7].arr, buffer + data[0].size + data[1].size + data[2].size + data[3].size + data[4].size + data[5].size, data[6].size, data[7].size);

        int* res5 = merge(res1, res2, arr, data[0].size + data[1].size, data[2].size + data[3].size);
        int* res6 = merge(res3, res4, arr + data[0].size + data[1].size + data[2].size + data[3].size, data[4].size + data[5].size, data[6].size + data[7].size);

        merge(res5, res6, buffer, data[0].size + data[1].size + data[2].size + data[3].size, data[4].size + data[5].size + data[6].size + data[7].size);
    }

    // Проверка на правильность
    int* result = threadCount == 4 ? arr : buffer;
}

