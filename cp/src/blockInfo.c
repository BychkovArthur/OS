#include "../include/blockInfo.h"

bool isBlockFree(BlockInfo* block) {
    return (size_t)block & (size_t)1;
}

BlockInfo* setBlockFree(BlockInfo* block) {
    return (BlockInfo*)((size_t)block | (size_t)1);
}

BlockInfo* setBlockOccupied(BlockInfo* block) {
    return (BlockInfo*)((size_t)block & ~(size_t)1);
}


BlockInfo* resetToNormalPointer(BlockInfo* block) {
    return setBlockOccupied(block);
}