#include "../include/testFunctionsRMAllocator.h"

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#include "../include/testSettings.h"
#include "../include/resourceMapAllocator.h"

size_t getMicrotime() {
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return (size_t)currentTime.tv_sec * (size_t)1e6 + (size_t)currentTime.tv_usec;
}

void testWithLightDataSequential(Allocator* allocator) {
    srand(time(NULL));

    size_t totalRequested = 0;
    size_t totalAllocated = 0;
    size_t totalTimeToAllocate = 0;
    size_t totalTimeToFree = 0;

    char** arrays = allocBlock(allocator, sizeof(char*) * LIGHT_DATA_SEQUENTIAL_ARRAY_SIZE);

    for (size_t iteration = 0; iteration < LIGHT_DATA_SEQUENTIAL_TEST_COUNT; ++iteration) {

        #ifdef INFO
        printf("Test number: %zu\n", iteration + 1);
        #endif

        size_t currentIterationStart = getMicrotime();

        // Выделяем
        for (size_t i = 0; i < LIGHT_DATA_SEQUENTIAL_ARRAY_SIZE; ++i) {

            #ifdef INFO
            printf("Allocating. I = %zu\n", i);
            #endif

            size_t currentSize = sizeof(char) * ((rand() % 16) + 1);
            arrays[i] = allocBlock(allocator, currentSize);
            totalRequested += currentSize;
            totalAllocated += getBlockLengthByGivenMemory(arrays[i]) + sizeof(BlockInfo);
        }

        // Добавляем время необходимое для выделения памяти
        totalTimeToAllocate += (getMicrotime() - currentIterationStart);

        size_t currentFreeStart = getMicrotime();

        for (size_t i = 0; i < LIGHT_DATA_SEQUENTIAL_ARRAY_SIZE; ++i) {

            #ifdef INFO
            printf("Free. I = %zu\n", i);
            #endif

            freeBlock(allocator, arrays[i]);
        }

        totalTimeToFree += (getMicrotime() - currentFreeStart);
    }

    printf("totalRequested: %zu\n", totalRequested);
    printf("totalAllocated: %zu\n", totalAllocated);
    printf("Microsecs to alloc: %zu\n", totalTimeToAllocate);
    printf("Microsecs to free: %zu\n", totalTimeToFree);

    freeBlock(allocator, arrays);
}

void testLightDataRandom(Allocator* allocator) {
    srand(time(NULL));

    size_t startTime = getMicrotime();

    size_t totalRequested = 0;
    size_t totalAllocated = 0;
    char** arrays = allocBlock(allocator, sizeof(char*) * LIGHT_DATA_RANDOM_ARRAY_SIZE);
    for (size_t i = 0; i < LIGHT_DATA_RANDOM_ARRAY_SIZE; ++i) {
        arrays[i] = NULL;
    }
#ifdef INFO
    printf("First block length: %zu\n", allocator->firstFreeBLock->blockSize);
#endif

    // Выделяем
    for (size_t i = 0; i < LIGHT_DATA_RANDOM_ARRAY_SIZE; ++i) {
#ifdef INFO
        printf("Allocating. I = %zu\n", i);
#endif
        size_t currentSize = sizeof(char) * ((rand() % 16) + 1);
        arrays[i] = allocBlock(allocator, currentSize);

        totalRequested += currentSize;
        totalAllocated += getBlockLengthByGivenMemory(arrays[i]) + sizeof(BlockInfo);

        // С вероятностью 1 / 8 будем очищать 5 элементов
        if (currentSize <= 2) {
            for (size_t j = 0; j < 5; ++j) {
                size_t indexToDelete = rand() % (i + 1);
                if (arrays[indexToDelete]) {
#ifdef INFO
                    printf("Free. I = %zu\n", indexToDelete);
#endif
                    freeBlock(allocator, arrays[indexToDelete]);
                    arrays[indexToDelete] = NULL;
                }

            }
        }
        
    }

    size_t timeAfterAlloc = getMicrotime();
    printf("Microsecs to alloc + some free: %zu\n", timeAfterAlloc - startTime);

    for (size_t i = 0; i < LIGHT_DATA_RANDOM_ARRAY_SIZE; ++i) {
        if (arrays[i]) {
#ifdef INFO
            printf("Free. I = %zu\n", i);
#endif
            freeBlock(allocator, arrays[i]);
            arrays[i] = NULL;
        }
    }
    freeBlock(allocator, arrays);

    size_t timeAfterFree = getMicrotime();
    printf("totalRequested: %zu\n", totalRequested);
    printf("totalAllocated: %zu\n", totalAllocated);
    printf("Microsecs to free left: %zu\n", timeAfterFree - timeAfterAlloc);
}