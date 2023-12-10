#include "../include/resourceMapAllocator.h"
#include "../include/blockInfo.h"

#include <stdio.h>

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

    printf("%p\n", ((BlockInfo*)allocator->memory)->nextBlock);
    printf("HERE 0 %p: \n", allocator->memory);
    printf("HERE %p\n", allocator->firstBlock);
    printf("%zu", getBlockLength(allocator, (BlockInfo*)allocator->memory));
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