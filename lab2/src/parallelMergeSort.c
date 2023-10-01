#include <pthread.h>
#include <stdio.h>
#include "../include/mergeSort.h"
#include "../include/parallelMergeSort.h"
#include "../include/structures.h"

#define BUFFER 0 
#define ARRAY 1


void* parallelMergeSortAlgorithm(void* dataForThread) {
    threadData_t* threadData = (threadData_t*) dataForThread;
    int* array = threadData->arr;
    int* buffer = threadData->buff;
    int size = threadData->size;
    int threadCount = threadData->threadCnt;
    
    // Отсортированный массив будет располагаться либо в buffer, либо в array
    // Я выбираю так, чтобы в итоге, после всех слияний в parallelMergeSort
    // отсортированный массив был в array
    int locationOfSortedArray = threadCount == 4 ? ARRAY : BUFFER;
    mergeSortAlgorithm(array, buffer, size, locationOfSortedArray);

    // for (int i = 0; i < size; ++i) {
    //     printf("buffer[i] = %d (th %ld)\n", buffer[i], pthread_self());
    // }
    // for (int i = 0; i < size; ++i) {
    //     printf("arr[i] = %d (th %ld)\n", array[i], pthread_self());
    // }
}

void parallelMergeSort(int threadCount, int* arr, int size) {
    if (!size) {
        return;
    }
    // Делаем буффер, чтобы в итоге отсортированный массив оказался именно в arr
    int buffer[size];
    for (int i = 0; i < size; ++i) {
        buffer[i] = arr[i];
    }

    pthread_t threads[threadCount];
    threadData_t data[threadCount];

    // Первые n-1 поток имею одинаковый размер массива, а последний то, что осталось
    for (int i = 0; i < threadCount - 1; ++i) {
        data[i].arr = arr + i * (size / threadCount);
        data[i].buff = buffer + i * (size / threadCount);
        data[i].size = size / threadCount;
        data[i].threadCnt = threadCount;
    }
    data[threadCount - 1].arr = arr + (threadCount - 1) * (size / threadCount);
    data[threadCount - 1].buff = buffer + (threadCount - 1) * (size / threadCount);
    data[threadCount - 1].size = size - (threadCount - 1) * (size / threadCount);
    data[threadCount - 1].threadCnt = threadCount;

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

    // Везде просто прибавляю сдвиг, чтобы писать в нужное место массива
    // Здесь последним (просто merge без записи) должен быть arr, чтобы именно в исходном массиве оказался полностью отсортированный массив
    // printf("HERE: %ld, %ld, %ld, %ld\n", data[0].arr, arr, data[1].arr, data + data[0].size);


    // for (int i = 0; i < size; ++i) {
    //     printf("1: buffer[i] = %d (th %ld)\n", buffer[i], pthread_self());
    // }
    // for (int i = 0; i < size; ++i) {
    //     printf("1: arr[i] = %d (th %ld)\n", arr[i], pthread_self());
    // }



    if (threadCount == 2) {
        // В buff лежит отсортированный массив для каждого потока
        merge(data[0].buff, data[1].buff, arr, data[0].size, data[1].size);

    } else if (threadCount == 4) {
        // В arr лежит отсортированный массив для каждого потока
        int* res1 = merge(data[0].arr, data[1].arr, buffer, data[0].size, data[1].size);
        int* res2 = merge(data[2].arr, data[3].arr, buffer + data[0].size + data[1].size, data[2].size, data[3].size);
        merge(res1, res2, arr, data[0].size + data[1].size, data[2].size + data[3].size);

    } else if (threadCount == 6) {
        // В buff лежит отсортированный массив для каждого потока
        int* res1 = merge(data[0].buff, data[1].buff, arr, data[0].size, data[1].size);
        int* res2 = merge(data[2].buff, data[3].buff, arr + data[0].size + data[1].size, data[2].size, data[3].size);
        int* res3 = merge(data[4].buff, data[5].buff, arr + data[0].size + data[1].size + data[2].size + data[3].size, data[4].size, data[5].size);

        int* res4 = merge(res1, res2, buffer, data[0].size + data[1].size, data[2].size + data[3].size);
        merge(res4, res3, arr, data[0].size + data[1].size + data[2].size + data[3].size, data[4].size + data[5].size);

    } else if (threadCount == 8) {
        // В buff лежит отсортированный массив для каждого потока
        int* res1 = merge(data[0].buff, data[1].buff, arr, data[0].size, data[1].size);
        int* res2 = merge(data[2].buff, data[3].buff, arr + data[0].size + data[1].size, data[2].size, data[3].size);
        int* res3 = merge(data[4].buff, data[5].buff, arr + data[0].size + data[1].size + data[2].size + data[3].size, data[4].size, data[5].size);
        int* res4 = merge(data[6].buff, data[7].buff, arr + data[0].size + data[1].size + data[2].size + data[3].size + data[4].size + data[5].size, data[6].size, data[7].size);

        int* res5 = merge(res1, res2, buffer, data[0].size + data[1].size, data[2].size + data[3].size);
        int* res6 = merge(res3, res4, buffer + data[0].size + data[1].size + data[2].size + data[3].size, data[4].size + data[5].size, data[6].size + data[7].size);

        merge(res5, res6, arr, data[0].size + data[1].size + data[2].size + data[3].size, data[4].size + data[5].size + data[6].size + data[7].size);
    }
}
