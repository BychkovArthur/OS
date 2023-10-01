
#include "../include/mergeSort.h"

// Спросить, это колхоз или как?

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

int* mergeSort(int* array, int size) {
    if (size) {
        int buffer[size];
        for (int i = 0; i < size; ++i) {buffer[i] = array[i];} // Нужно заполнять, т.к. на любом уровне (в т.ч. где 1 элемент) может быть как buff, так и arr, даже для одного массива (например, если длина 7)
        // В этом случае, может возникнуть конфликт того, что я буду в merge писать или из buff в buff или из arr в arr
        mergeSortAlgorithm(array, buffer, size, 1);
    }
}
