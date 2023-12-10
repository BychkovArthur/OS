#include "../include/resourceMapAllocator.h"
#include "../include/blockInfo.h"

#include <stdio.h>
#include <stdint.h>

void printBinary(size_t num) {
    int bits = sizeof(size_t) * 8; // Количество бит в int
    for (int i = bits - 1; i >= 0; i--) {
        int bit = (num >> i) & 1;
        printf("%d", bit);
    }
    printf("\n");
}

int main() {
    size_t d;
    scanf("%zu", &d);
    Allocator* allocator = createMemoryAllocator(d);
    BlockInfo* firstBlock = ((BlockInfo*)allocator->memory);
    printf("%p\n", (void*)firstBlock->nextBlock);
    printf("HERE 0 %p: \n", allocator->memory);
    printf("HERE %p\n", (void*)firstBlock);
    printf("%zu\n", getBlockLengthByBlock(allocator, (BlockInfo*)allocator->memory));

    void* alloc1 = allocBlock(allocator, 10);
    
    void* alloc2 = allocBlock(allocator, 30);
    void* alloc3 = allocBlock(allocator, 16);
    printf("Начало памяти       : %ld\nНачало первого блока: %ld\nНачало второго блока: %ld\n", allocator->memory, alloc1 - allocator->memory, alloc2 - allocator->memory);
    printf("Размер первого блока: %zu\n", getBlockLengthByGivenMemory(allocator, alloc1));
    printf("Размер второго блока: %zu\n", getBlockLengthByGivenMemory(allocator, alloc2));
    printf("Размер третьего блока: %zu\n", getBlockLengthByBlock(allocator, (BlockInfo*)(alloc2 + 32)));
    printf("Размер третьего блока: %zu\n", getBlockLengthByGivenMemory(allocator, (void*)((uint8_t*)alloc2 + 40)));
    printf("HERE %ld\n", (alloc2 + 32) - allocator->memory);
    printf("ptr: %p\n", ((BlockInfo*)(alloc2 + 32))->nextBlock);
    // BlockInfo block = {
    //     NULL,
    // };
    // BlockInfo block2 = {
    //     &block,
    // };

    // printBinary((size_t)block.nextBlock);
    // printBinary((size_t)block2.nextBlock);
    // printf("%d\n", isBlockFree(block2.nextBlock));
    // block2.nextBlock = setBlockFree(block2.nextBlock);
    // printBinary((size_t)block2.nextBlock);
    // printf("%d\n", isBlockFree(block2.nextBlock));
    // block2.nextBlock = setBlockOccupied(block2.nextBlock);
    // printBinary((size_t)block2.nextBlock);
    // printf("%d\n", isBlockFree(block2.nextBlock));
}