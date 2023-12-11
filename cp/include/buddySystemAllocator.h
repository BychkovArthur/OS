#ifndef __BUDDY_SYSTEM_ALLOCATOR__
#define __BUDDY_SYSTEM_ALLOCATOR__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MIN_BLOCK_SIZE 32
#define MIN_POWER_OF_TWO 5
#define MAX_POWER_OF_TWO 64

typedef struct _BlockInfo {
    uint32_t size;
    uint32_t isFree;
    struct _BlockInfo* next;
    struct _BlockInfo* prev;
} BlockInfo;

typedef struct _Allocator {
    void* memory;
    size_t memorySize;
} Allocator;

size_t align(size_t);
size_t getPowerOf2(size_t);
void initArrayOfBlocks();


Allocator* createMemoryAllocator(size_t);
void* allocBlock(Allocator*, size_t);
void* recursiveAlloc(Allocator*, size_t, size_t, size_t);
void freeBlock(const Allocator*, void*);


#endif // __BUDDY_SYSTEM_ALLOCATOR__