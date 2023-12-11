#include "../include/resourceMapAllocator.h"
#include "../include/blockInfo.h"

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
    printf("Длина всего: %zu\n", getBlockLengthByBlock(allocator, (BlockInfo*)allocator->memory));

    void* alloc1 = allocBlock(allocator, 10);
    
    void* alloc2 = allocBlock(allocator, 30);
    void* alloc3 = allocBlock(allocator, 33);
    printf("HEREalksdjfl;askjfklajsl;kjf;alksjdf\n");
    printf("Начало памяти       : %ld\nНачало первого блока: %ld\nНачало второго блока: %ld\n", allocator->memory, alloc1 - allocator->memory, alloc2 - allocator->memory);
    printf("Размер первого блока: %zu\n", getBlockLengthByGivenMemory(allocator, alloc1));
    printf("Размер второго блока: %zu\n", getBlockLengthByGivenMemory(allocator, alloc2));
    printf("Размер третьего блока: %zu\n", getBlockLengthByBlock(allocator, (BlockInfo*)(alloc2 + 32)));
    printf("Размер третьего блока: %zu\n", getBlockLengthByGivenMemory(allocator, (void*)((uint8_t*)alloc2 + 40)));
    printf("HERE %ld\n", (alloc2 + 32) - allocator->memory);
    printf("ptr: %p\n", ((BlockInfo*)(alloc2 + 32))->nextBlock);
    printf("Размер 4-го блока: %ld", getBlockLengthByGivenMemory(allocator, alloc3 + 40));
    printf("\n\n\n");

    // allocBlock(allocator, 5);

    freeBlock(allocator, alloc3);
    printf("Длина всего3: %zu\n", getBlockLengthByBlock(allocator, (BlockInfo*)allocator->memory));
    freeBlock(allocator, alloc1);
    printf("Длина всего1: %zu\n", getBlockLengthByBlock(allocator, (BlockInfo*)allocator->memory));
    freeBlock(allocator, alloc2);
    printf("Длина всего2: %zu\n", getBlockLengthByBlock(allocator, (BlockInfo*)allocator->memory));
    // printf("%d\n", isBlockFree((BlockInfo*)(alloc1 - ALIGN_BY)));
    // printf("%d\n", isBlockFree((BlockInfo*)(alloc2 - ALIGN_BY)));
    // printf("%d\n", isBlockFree((BlockInfo*)(alloc3 - ALIGN_BY)));

    void* alloc4 = allocBlock(allocator, 24);
    
    void* alloc5 = allocBlock(allocator, 65);
    printf("\n\n\n\n");
    printf("Начало памяти       : %ld\nНачало первого блока: %ld\nНачало второго блока: %ld\n", allocator->memory, alloc4 - allocator->memory, alloc5 - allocator->memory);
    printf("Размер первого блока: %zu\n", getBlockLengthByGivenMemory(allocator, alloc4));
    printf("Размер второго блока: %zu\n", getBlockLengthByGivenMemory(allocator, alloc5));
    printf("Последний блок: %p", ((BlockInfo*)(alloc5 - ALIGN_BY))->nextBlock);
    printf("Последний блок: %p", ((BlockInfo*)(alloc5 - ALIGN_BY))->nextBlock);
    // void* alloc3 = allocBlock(allocator, 33);
}