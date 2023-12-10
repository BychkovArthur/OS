#include "../include/blockInfo.h"

bool isBlockFree(BlockInfo* block) {
    return ((size_t)(block->nextBlock) & (size_t)7);
}

BlockInfo* setBlockFree(BlockInfo* block) {
    return (BlockInfo*)((size_t)(block->nextBlock) | (size_t)7);
}

BlockInfo* setBlockOccupied(BlockInfo* block) {
    return (BlockInfo*)((size_t)(block->nextBlock) & ~(size_t)7);
}


BlockInfo* resetToNormalPointer(BlockInfo* block) {
    return (BlockInfo*)((size_t)(block) & ~(size_t)7);
}