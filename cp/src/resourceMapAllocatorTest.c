#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#include "../include/resourceMapAllocator.h"

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
        printf("I = %zu\n", i);
        size_t currentSize = sizeof(char) * ((rand() % 16) + 1);
        arrays[i] = allocBlock(allocator, currentSize);
        printf(".\n");
        totalRequested += currentSize;
        totalAllocated += getBlockLengthByGivenMemory(arrays[i]) + sizeof(BlockInfo);
    }

    size_t timeAfterAlloc = getMicrotime();
    printf("totalRequested: %zu\n", totalRequested);
    printf("totalAllocated: %zu\n", totalAllocated);
    printf("Microsecs to alloc: %zu\n", timeAfterAlloc - startTime);

    printf("HERE\n");
    for (size_t i = 0; i < 1000000; ++i) {
        freeBlock(allocator, arrays[i]);
    }
    freeBlock(allocator, arrays);
    size_t timeAfterFree = getMicrotime();
    printf("Microsecs to free: %zu\n", timeAfterFree - timeAfterAlloc);
}

// Тестирование на выделение с одновременным освобождением данные до 16 байт
void testLightDataRandom(Allocator* allocator) {
    srand(time(NULL));

    size_t startTime = getMicrotime();

    size_t totalRequested = 0;
    size_t totalAllocated = 0;
    char** arrays = allocBlock(allocator, sizeof(char*) * 100000);
    for (size_t i = 0; i < 100000; ++i) {
        arrays[i] = NULL;
    }

    printf("%zu\n", allocator->firstFreeBLock->blockSize);

    // Выделяем
    for (size_t i = 0; i < 100000; ++i) {
        printf("I = %zu\n", i);
        size_t currentSize = sizeof(char) * ((rand() % 16) + 1);
        // printf("HERE alloc1\n");
        arrays[i] = allocBlock(allocator, currentSize);
        // printf("HERE alloc2\n");

        totalRequested += currentSize;
        // printf("crash1\n");
        totalAllocated += getBlockLengthByGivenMemory(arrays[i]) + sizeof(BlockInfo);
        // printf("crash2\n");

        // С вероятностью 1 / 8 будем очищать 5 элементов
        if (currentSize <= 2) {
            for (size_t j = 0; j < 5; ++j) {
                size_t indexToDelete = rand() % (i + 1);
                if (arrays[indexToDelete]) {
                    // printf("HERE free1\n");
                    freeBlock(allocator, arrays[indexToDelete]);
                    // printf("HERE free 2\n");
                    arrays[indexToDelete] = NULL;
                    // printf("HERE free 3\n");
                    // printf("DELETED ----------- %zu\n", indexToDelete);
                }

            }
            // printf("out\n");
        }
        
        
    }

    for (size_t i = 0; i < 100000; ++i) {
        printf("I = %zu\n", i);
        if (arrays[i]) {
            freeBlock(allocator, arrays[i]);
            arrays[i] = NULL;
        }
    }
    freeBlock(allocator, arrays);

    size_t timeAfterAlloc = getMicrotime();
    printf("totalRequested: %zu\n", totalRequested);
    printf("totalAllocated: %zu\n", totalAllocated);
    printf("Microsecs total: %zu\n", timeAfterAlloc - startTime);
}

int main() {
    Allocator* allocator = createMemoryAllocator(40000000 + 100);

    // testWithLightData(allocator);
    testLightDataRandom(allocator);
    void* a = allocBlock(allocator, 40000000);
    // void* a = allocBlock(allocator, 4000000 - 1000000);
}