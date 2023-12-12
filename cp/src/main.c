#include "../include/resourceMapAllocator.h"
#include "../include/resourceMapBlockInfo.h"

#include <stdio.h>
#include <stdint.h>

// void printBinary(size_t num) {
//     int bits = sizeof(size_t) * 8; // Количество бит в int
//     for (int i = bits - 1; i >= 0; i--) {
//         int bit = (num >> i) & 1;
//         printf("%d", bit);
//     }
//     printf("\n");
// }

int main() {
    size_t d;
    scanf("%zu", &d);
    Allocator* allocator = createMemoryAllocator(d);
    BlockInfo* firstBlock = ((BlockInfo*)allocator->memory);
    printf("%p\n", (void*)firstBlock->nextBlock);
    printf("HERE 0 %p: \n", allocator->memory);
    printf("HERE %p\n", (void*)firstBlock);
    printf("Длина всего: %zu\n", ((BlockInfo*)allocator->memory)->blockSize);

    void* alloc1 = allocBlock(allocator, 10);
    
    void* alloc2 = allocBlock(allocator, 30);
    void* alloc3 = allocBlock(allocator, 33);
    void* alloc4 = allocBlock(allocator, 832);
    printf("Вызов 5\n");
    void* alloc5 = allocBlock(allocator, 1);

    
    printf("Начало первого блока: %zu\n", alloc1 - allocator->memory);
    printf("Начало второго блока: %zu\n", alloc2 - allocator->memory);
    printf("Начало третьего блока: %zu\n", alloc3 - allocator->memory);
    printf("Начало четвертого блока: %zu\n", alloc4 - allocator->memory);
    printf("Начало 5 блока: %zu\n", alloc5 - allocator->memory);
    // printf("Начало памяти       : %ld\nНачало первого блока: %ld\nНачало второго блока: %ld\n", allocator->memory, alloc1 - allocator->memory, alloc2 - allocator->memory);
    printf("Размер первого блока: %zu\n", getBlockLengthByGivenMemory(alloc1));
    printf("Размер второго блока: %zu\n", getBlockLengthByGivenMemory(alloc2));
    printf("Размер третьего блока: %zu\n", getBlockLengthByGivenMemory(alloc3));
    printf("Размер четвертого блока: %zu\n", getBlockLengthByGivenMemory(alloc4));
    printf("Размер 5 блока: %zu\n", getBlockLengthByGivenMemory(alloc5));
    // printf("Размер третьего блока: %zu\n", ((BlockInfo*)(alloc2 + 32))->blockSize);
    // printf("Размер третьего блока: %zu\n", getBlockLengthByGivenMemory((void*)((uint8_t*)alloc2 + 40)));
    // printf("HERE %ld\n", (alloc2 + 32) - allocator->memory);
    // printf("ptr: %p\n", ((BlockInfo*)(alloc2 + 32))->nextBlock);
    // printf("Начало 3-го блока :%zu\nНачало 4-го блока %zu\n", alloc3 - allocator->memory, alloc3 - allocator->memory);
    printf("Размер 4-го блока: %ld\n", getBlockLengthByGivenMemory(alloc3 + 48 + 16));
    printf("%zu\n", allocator->firstFreeBLock);
    printf("\n\n\n");

    // allocBlock(allocator, 5);

    printf("До освобождения %zu\n", allocator->firstFreeBLock);
    freeBlock(allocator, alloc3);
    printf("ПОСЛЕ 1 %zu\n", getOffset(allocator, allocator->firstFreeBLock));
    printf("После: %zu\n", getOffset(allocator, alloc3 - 16));
    printf("После длина: %zu\n", getBlockLengthByGivenMemory(alloc3));
    printf("Свободен: %zu\n", isBlockFree(alloc3 - 16));
    printf("\n\n\n");

    printf("До освобождения %zu\n", getOffset(allocator, allocator->firstFreeBLock));
    freeBlock(allocator, alloc1);
    printf("ПОСЛЕ 1 %zu\n", getOffset(allocator, allocator->firstFreeBLock));
    printf("После: %zu\n", getOffset(allocator, alloc1 - 16));
    printf("После длина: %zu\n", getBlockLengthByGivenMemory(alloc1));
    printf("Свободен: %zu\n", isBlockFree(alloc1 - 16));
    printf("\n\n\n");


    printf("До освобождения %zu\n", getOffset(allocator, allocator->firstFreeBLock));
    freeBlock(allocator, alloc2);
    printf("ПОСЛЕ 1 %zu\n", getOffset(allocator, allocator->firstFreeBLock));
    printf("После: %zu\n", getOffset(allocator, alloc2 - 16));
    printf("После длина: %zu\n", getBlockLengthByGivenMemory(alloc2));
    printf("Свободен: %zu\n", isBlockFree(alloc2 - 16));
    printf("\n\n\n");

    void* alloc6 = allocBlock(allocator, 128);
    printf("Начало 6 блока: %zu\n", alloc6 - allocator->memory);
    printf("Размер 6 блока: %zu\n", getBlockLengthByGivenMemory(alloc6));
    printf("ПОСЛЕ 1 %p\n", allocator->firstFreeBLock);
    printf("\n\n\n");

    printf("До освобождения %zu\n", getOffset(allocator, allocator->firstFreeBLock));
    freeBlock(allocator, alloc5);
    printf("ПОСЛЕ 1 %zu\n", getOffset(allocator, allocator->firstFreeBLock));
    printf("После: %zu\n", getOffset(allocator, alloc5 - 16));
    printf("После длина: %zu\n", getBlockLengthByGivenMemory(alloc5));
    printf("Свободен: %zu\n", isBlockFree(alloc5 - 16));
    printf("\n\n\n");

    printf("До освобождения %zu\n", getOffset(allocator, allocator->firstFreeBLock));
    freeBlock(allocator, alloc4);
    printf("ПОСЛЕ 1 %zu\n", getOffset(allocator, allocator->firstFreeBLock));
    printf("После: %zu\n", getOffset(allocator, alloc4 - 16));
    printf("После длина: %zu\n", getBlockLengthByGivenMemory(alloc4));
    printf("Свободен: %zu\n", isBlockFree(alloc4 - 16));
    printf("\n\n\n");

    printf("До освобождения %zu\n", getOffset(allocator, allocator->firstFreeBLock));
    freeBlock(allocator, alloc6);
    printf("ПОСЛЕ 1 %zu\n", getOffset(allocator, allocator->firstFreeBLock));
    printf("После: %zu\n", getOffset(allocator, alloc6 - 16));
    printf("После длина: %zu\n", getBlockLengthByGivenMemory(alloc6));
    printf("Свободен: %zu\n", isBlockFree(alloc6 - 16));
    printf("\n\n\n");
    
    printf("До освобождения %zu\n", getOffset(allocator, allocator->firstFreeBLock));
    alloc1 = allocBlock(allocator, 10);
    printf("ПОСЛЕ 1 %zu\n", getOffset(allocator, allocator->firstFreeBLock));
    printf("Начало первого блока: %zu\n", alloc1 - allocator->memory);
    printf("Размер первого блока: %zu\n", getBlockLengthByGivenMemory(alloc1));
    printf("Размер 2 блока: %zu\n", getBlockLengthByGivenMemory(alloc1 + 16));
    printf("Свободен: %zu\n", isBlockFree(alloc1 - 16));
    printf("Свободен: %zu\n", isBlockFree(alloc1 + 16));

    // printf("Длина всего3: %zu\n", getBlockLengthByBlock(allocator, (BlockInfo*)allocator->memory));
    // printf("Длина всего1: %zu\n", getBlockLengthByBlock(allocator, (BlockInfo*)allocator->memory));
    // freeBlock(allocator, alloc2);
    // printf("Длина всего2: %zu\n", getBlockLengthByBlock(allocator, (BlockInfo*)allocator->memory));
    // // printf("%d\n", isBlockFree((BlockInfo*)(alloc1 - ALIGN_BY)));
    // // printf("%d\n", isBlockFree((BlockInfo*)(alloc2 - ALIGN_BY)));
    // // printf("%d\n", isBlockFree((BlockInfo*)(alloc3 - ALIGN_BY)));

    // void* alloc4 = allocBlock(allocator, 24);
    
    // void* alloc5 = allocBlock(allocator, 65);
    // printf("\n\n\n\n");
    // printf("Начало памяти       : %ld\nНачало первого блока: %ld\nНачало второго блока: %ld\n", allocator->memory, alloc4 - allocator->memory, alloc5 - allocator->memory);
    // printf("Размер первого блока: %zu\n", getBlockLengthByGivenMemory(alloc4));
    // printf("Размер второго блока: %zu\n", getBlockLengthByGivenMemory(alloc5));
    // printf("Последний блок: %p", ((BlockInfo*)(alloc5 - ALIGN_BY))->nextBlock);
    // printf("Последний блок: %p", ((BlockInfo*)(alloc5 - ALIGN_BY))->nextBlock);
    // // void* alloc3 = allocBlock(allocator, 33);
}