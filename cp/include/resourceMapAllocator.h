#ifndef __RESOURCE_MAP_ALLOCATOR__
#define __RESOURCE_MAP_ALLOCATOR__

#include <stddef.h>

#include "blockInfo.h"

#define ALIGN_BY (sizeof(BlockInfo))

typedef struct _Allocator {
    void* memory;
    size_t memorySize;
} Allocator;

size_t align(size_t);

Allocator* createMemoryAllocator(size_t);
void* allocBlock(Allocator*, size_t);
void freeBlock(const Allocator*, void*);
size_t getBlockLengthByBlock(Allocator*, BlockInfo*);
size_t getBlockLengthByGivenMemory(Allocator*, void*);
void concatenateBlocks(Allocator*);

#endif // __RESOURCE_MAP_ALLOCATOR__