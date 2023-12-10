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

Allocator* createMemoryAllocator(size_t memorySize);
void* allocBlock(Allocator* allocator, size_t blockSize);
void* freeBlock(Allocator* allocator, void* block);
size_t getBlockLength(Allocator*, BlockInfo* block);

#endif // __RESOURCE_MAP_ALLOCATOR__