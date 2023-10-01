#include <stdio.h>
#include <stdlib.h>
#include "../include/mergeSort.h"
#include <time.h>
#include <pthread.h>

// Спросить, это колхоз или как?

int* mergeSortAlgorithm(int* array, int* buffer, unsigned int size, int isLevelNumberEven) 
{
    if (size == 1) {
        return isLevelNumberEven ? array : buffer;
    }
    int* res1 = mergeSortAlgorithm(&array[0], &buffer[0], size / 2, (isLevelNumberEven + 1) % 2);
    int* res2 = mergeSortAlgorithm(&array[size / 2], &buffer[size / 2], size - (size / 2), (isLevelNumberEven + 1) % 2);

    int* result;
    if (isLevelNumberEven) {
        result = merge(res1, res2, &array[0], size / 2, size - (size / 2));
    } else {

        result = merge(res1, res2, &buffer[0], size / 2, size - (size / 2));
    }
    return result;
}

int* merge(int* array1, int* array2, int* buffer, const unsigned int size1, const unsigned int size2)
{
    unsigned int ptr1 = 0;
    unsigned int ptr2 = 0;

    while (1) {
        if (ptr1 < size1 && ( (ptr2 < size2 && array1[ptr1] <= array2[ptr2]) || (ptr2 == size2))) {
            buffer[ptr1 + ptr2] = array1[ptr1];
            ++ptr1;
        } else if (ptr2 < size2 && ( (ptr1 < size1 && array2[ptr2] <= array1[ptr1]) || (ptr1 == size1))) {
            buffer[ptr1 + ptr2] = array2[ptr2];
            ++ptr2;
        } else {
            break;
        }
    }
    return buffer;
}

int* mergeSort(int* array, int size) {
    if (size) {
        int buffer[size];
        for (int i = 0; i < size; ++i) {buffer[i] = array[i];} // Нужно заполнять, т.к. на любом уровне (в т.ч. где 1 элемент) может быть как buff, так и arr, даже для одного массива (например, если длина 7)
        // В этом случае, может возникнуть конфликт того, что я буду в merge писать или из buff в buff или из arr в arr
        mergeSortAlgorithm(array, buffer, size, 1);
    }
}







int correctSort(int* arr, int size) {
    for (int i = 0; i < size - 1; ++i) {
        if (arr[i] > arr[i + 1]) {return 0;}
    }
    return 1;
}













struct dataForThread
{
    int* arr;
    int size;
};

void* parallelMergeSort(void* dataForThread) {
    struct dataForThread* data = (struct dataForThread*) dataForThread;
    int* array = data->arr;
    int size = data->size;
    if (size) {
        int buffer[size];
        for (int i = 0; i < size; ++i) {buffer[i] = array[i];} // Нужно заполнять, т.к. на любом уровне (в т.ч. где 1 элемент) может быть как buff, так и arr, даже для одного массива (например, если длина 7)
        // В этом случае, может возникнуть конфликт того, что я буду в merge писать или из buff в buff или из arr в arr
        mergeSortAlgorithm(array, buffer, size, 1);
    }
}


int test = 0;

void parallelMergeSortAlgorithm(int threadCount, int* arr, int size) {
    pthread_t threads[threadCount];
    struct dataForThread data1, data2;
    data1.arr = arr;
    data1.size = size / 2;
    data2.arr = arr + (size / 2);
    data2.size = size - (size / 2);
    
    if (pthread_create(threads, NULL, &parallelMergeSort, &data1)) {
        perror("Thread create error");
    }
    if (pthread_create(threads + 1, NULL, &parallelMergeSort, &data2)) {
        perror("Thread create error");
    }


    for (int i = 0; i < 2; ++i) {
        if (pthread_join(threads[i], NULL)) {
            perror("Thread join error");
        }
    }

    int buffer[size];
    merge(arr, arr + (size / 2), buffer, size / 2, size - (size / 2));



    // Проверка на правильность
    if (correctSort(buffer, size)) {
        printf("Проверка №%d УСПЕХ\n", ++test);
    } else {
        printf("Проверка №%d ОШИБКА\n", ++test);
    }
    for (int j = 0; j < size; ++j) {
        printf("%d ", buffer[j]);
    }
    printf("\n");
}

// 18 секунд - 1 потока
// 14 секунд - 2 поток
#define SIZE 10
int main(int argc, char* argv[]) {
    time_t start = time(NULL);
    
    int arr[SIZE];


    for (int i = 0; i < 100; ++i) {

        for (int i = 0; i < SIZE; ++i) {
            arr[i] = rand() % 20;
        }

        if (argc == 2) {
            parallelMergeSortAlgorithm(2, arr, SIZE);
        } else {
            mergeSort(arr, SIZE);
            if (correctSort(arr, SIZE)) {
                printf("Проверка №%d УСПЕХ\n", i + 1);
            } else {
                printf("Проверка №%d ОШИБКА\n", i + 1);
                for (int j = 0; j < SIZE; ++j) {
                    printf("%d ", arr[j]);
                }
                printf("\n");
            }
        }

    }
    time_t end = time(NULL);
    printf("Seconds: %ld\n", end - start);
}