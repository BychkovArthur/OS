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

bool isBlockFree(BlockInfo* block) {
    // printf("IN IS FREE %zu\n", (size_t)(block->nextBlock) & (size_t)7);
    // printBinary(((size_t)(block->nextBlock) & (size_t)7));
    return ((size_t)(block->nextBlock) & (size_t)7);
}

BlockInfo* setBlockFree(BlockInfo* block) {
    // printf("IN SET FREE\n");
    // printBinary((size_t)(block->nextBlock) | (size_t)7);
    return (BlockInfo*)((size_t)(block->nextBlock) | (size_t)7);
}

BlockInfo* setBlockOccupied(BlockInfo* block) {
    // printf("IN SET OCCUPIED\n");
    // printBinary(((size_t)(block->nextBlock) & ~(size_t)7));
    return (BlockInfo*)((size_t)(block->nextBlock) & ~(size_t)7);
}


BlockInfo* resetToNormalPointer(BlockInfo* block) {
    return (BlockInfo*)((size_t)(block) & ~(size_t)7);
}