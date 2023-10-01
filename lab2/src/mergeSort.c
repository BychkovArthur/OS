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
    struct dataForThread data[threadCount];

    // Первые n-1 поток имею одинаковый размер массива, а последний то, что осталось
    for (int i = 0; i < threadCount - 1; ++i) {
        data[i].arr = arr + i * (size / threadCount);
        data[i].size = size / threadCount;
    }
    data[threadCount - 1].arr = arr + (threadCount - 1) * (size / threadCount);
    data[threadCount - 1].size = size - (threadCount - 1) * (size / threadCount);

    // Создание потоков
    for (int i = 0; i < threadCount; ++i) {
        if (pthread_create(&threads[i], NULL, &parallelMergeSort, &data[i])) {
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
    

    if (threadCount == 1) {

    // Везде просто прибавляю сдвиг, чтобы писать в нужное место массива
    } else if (threadCount == 2) {
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
    if (correctSort(result, size)) {
        printf("Проверка №%d УСПЕХ\n", ++test);
    } else {
        printf("Проверка №%d ОШИБКА\n", ++test);
        for (int j = 0; j < size; ++j) {
            printf("%d ", result[j]);
        }
        printf("\n");
    }
}

// ryzen 2600x (6 ядер, 12 потоков)
// 1 поток ------ массив на 1000000 сортируется 100 раз: 18 секунда
// 2 потока ------ массив на 1000000 сортируется 100 раз: 14 секунда
// 4 потока ------ массив на 1000000 сортируется 100 раз: 10 секунда
// 6 потоков ------ массив на 1000000 сортируется 100 раз: 8 секунда
// 8 потоков ------ массив на 1000000 сортируется 100 раз: 9 секунда

#define SIZE 1000000
#define MAX_NUMBER 2000000
int main(int argc, char* argv[]) {
    time_t start = time(NULL);
    
    int arr[SIZE];


    for (int i = 0; i < 100; ++i) {

        for (int i = 0; i < SIZE; ++i) {
            arr[i] = rand() % MAX_NUMBER;
        }

        if (argc == 2) {
            parallelMergeSortAlgorithm(6, arr, SIZE);
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