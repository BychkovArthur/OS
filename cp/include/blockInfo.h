#ifndef __BLOCK_INFO__
#define __BLOCK_INFO__

#include <stdlib.h>
#include <stdbool.h>

typedef struct _BlockInfo {
    struct _BlockInfo* nextBlock;
} BlockInfo;

bool isBlockFree(BlockInfo*);
BlockInfo* setBlockFree(BlockInfo*);
BlockInfo* setBlockOccupied(BlockInfo*);
BlockInfo* resetToNormalPointer(BlockInfo*);

bool adjacentBlocksExistsAndFree(BlockInfo*);
bool currentBlockBetter(BlockInfo*, size_t, size_t, size_t);

#endif // __BLOCK_INFO__