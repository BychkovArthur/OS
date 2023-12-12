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


    // time_t start = time(NULL);
    // struct timeval currentTime;
	// gettimeofday(&currentTime, NULL);
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
        totalAllocated += getBlockLengthByGivenMemory(arrays[i]);
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

int main() {
    Allocator* allocator = createMemoryAllocator(40000000);

    testWithLightData(allocator);

    // int* arr = allocBlock(allocator, sizeof(int) * 100000);
    // for (int i = 0; i < 100000; ++i) {
    //     arr[i] = i;
    // }
    // size_t res = 0;

    // FILE* file = fopen("test.txt", "w");

    // for (int i = 0; i < 100000; ++i) {
    //     fprintf(file, "%d", arr[i]);
    // }
    // printf("Res: %zu\n", res);
    // sleep(5);
    // destroyMemoryAllocator(allocator);
    // return 0;
}