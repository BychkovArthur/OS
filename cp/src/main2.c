#include "../include/buddySystemAllocator.h"

#include <stdio.h>

int main() {
    size_t a;
    scanf("%zu", &a);
    Allocator* allocator = createMemoryAllocator(a);
    printf("MEMORY START -------------- %p\n\n", allocator->memory);
    // printf("HERE %zu\n", allocBlock(allocator, 1000) - allocator->memory);
    printf("HERE %zu\n", allocBlock(allocator, 256 - 24) - allocator->memory);
    // printf("HERE %zu\n", allocBlock(allocator, 512 - 24) - allocator->memory);
    printf("HERE %zu\n", allocBlock(allocator, 64 - 24) - allocator->memory);
    printf("HERE %zu\n", allocBlock(allocator, 32 - 24) - allocator->memory);
    printf("HERE %zu\n", allocBlock(allocator, 256 - 24) - allocator->memory);
    printf("HERE %zu\n", allocBlock(allocator, 128 - 24) - allocator->memory);
    printf("HERE %zu\n", allocBlock(allocator, 64 - 24) - allocator->memory);
    printf("HERE %zu\n", allocBlock(allocator, 32 - 24) - allocator->memory);
    printf("HERE %zu\n", allocBlock(allocator, 128 - 24) - allocator->memory);
    printf("HERE %zu\n", allocBlock(allocator, 64 - 24) - allocator->memory);
    printf("HERE %zu\n", allocBlock(allocator, 1) - allocator->memory);
}