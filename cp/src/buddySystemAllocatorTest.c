#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdlib.h>

#include "../include/buddySystemAllocator.h"

size_t getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return (size_t)currentTime.tv_sec * (size_t)1e6 + (size_t)currentTime.tv_usec;
}

// Тестирование на данных меньших, чем 16 байт
void testWithLightData(Allocator* allocator) {
    srand(time(NULL));
    size_t startTime = getMicrotime();

    size_t totalRequested = 0;
    size_t totalAllocated = 0;
    char** arrays = allocBlock(allocator, sizeof(char*) * 1000000);

    // Выделяем
    for (size_t i = 0; i < 1000000; ++i) {
        if (i % 100 == 0) {
            printf("I = %zu\n", i);

        }
        size_t currentSize = sizeof(char) * ((rand() % 16) + 1);
        arrays[i] = allocBlock(allocator, currentSize);
        totalRequested += currentSize;
        // totalAllocated += getBlockLengthByGivenMemory(allocator, arrays[i]);
        totalAllocated += ((BlockInfo*)((uint8_t*)arrays[i] - sizeof(BlockInfo)))->size;

    }

    size_t timeAfterAlloc = getMicrotime();
    printf("totalRequested: %zu\n", totalRequested);
    printf("totalAllocated: %zu\n", totalAllocated);
    printf("Microsecs to alloc: %zu\n", timeAfterAlloc - startTime);

    for (size_t i = 0; i < 1000000; ++i) {
        freeBlock(allocator, arrays[i]);
    }
    freeBlock(allocator, arrays);

    size_t timeAfterFree = getMicrotime();
    printf("Microsecs to free: %zu\n", timeAfterFree - timeAfterAlloc);
}


// Тестирование на данных от 16 до 512 байт
void testWithMiddleData(Allocator* allocator) {
    srand(time(NULL));
    size_t startTime = getMicrotime();

    size_t totalRequested = 0;
    size_t totalAllocated = 0;
    char** arrays = allocBlock(allocator, sizeof(char*) * 100000);

    // Выделяем
    for (size_t i = 0; i < 100000; ++i) {
        if (i % 100 == 0) {
            printf("I = %zu\n", i);

        }
        size_t currentSize = sizeof(char) * ((rand() % 496) + 17);
        arrays[i] = allocBlock(allocator, currentSize);
        totalRequested += currentSize;
        totalAllocated += ((BlockInfo*)((uint8_t*)arrays[i] - sizeof(BlockInfo)))->size;

    }

    size_t timeAfterAlloc = getMicrotime();
    printf("totalRequested: %zu\n", totalRequested);
    printf("totalAllocated: %zu\n", totalAllocated);
    printf("Microsecs to alloc: %zu\n", timeAfterAlloc - startTime);

    for (size_t i = 0; i < 100000; ++i) {
        freeBlock(allocator, arrays[i]);
    }
    freeBlock(allocator, arrays);

    size_t timeAfterFree = getMicrotime();
    printf("Microsecs to free: %zu\n", timeAfterFree - timeAfterAlloc);
}


int main() {
    Allocator* allocator = createMemoryAllocator(300000000);

    // testWithLightData(allocator);
    testWithMiddleData(allocator);
}