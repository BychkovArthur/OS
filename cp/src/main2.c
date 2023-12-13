#include "../include/buddySystemAllocator.h"

#include <stdio.h>

int main() {
    size_t a;
    scanf("%zu", &a);
    Allocator* allocator = createMemoryAllocator(a);


    void* m1 = allocBlock(allocator, 256 - 24);
    // printf("\n\n\n\n\n");
    // printBlocks(allocator);
    void* m2 = allocBlock(allocator, 64 - 24);
    void* m3 = allocBlock(allocator, 32 - 24);
    void* m4 = allocBlock(allocator, 256 - 24);
    void* m5 = allocBlock(allocator, 128 - 24);
    void* m6 = allocBlock(allocator, 64 - 24);
    void* m7 = allocBlock(allocator, 32 - 24);
    void* m8 = allocBlock(allocator, 128 - 24);
    void* m9 = allocBlock(allocator, 64 - 24);


    printf("MEMORY START -------------- %p\n\n", allocator->memory);
    // printf("HERE %zu\n", allocBlock(allocator, 1000) - allocator->memory);
    // printf("HERE %zu\n", allocBlock(allocator, 512 - 24) - allocator->memory);
    printf("HERE1 %zu\n", m1 - allocator->memory);
    printf("HERE2 %zu\n", m2 - allocator->memory);
    printf("HERE3 %zu\n", m3 - allocator->memory);
    printf("HERE4 %zu\n", m4 - allocator->memory);
    printf("HERE5 %zu\n", m5 - allocator->memory);
    
    printf("HERE6 %zu\n", m6 - allocator->memory);
    printf("HERE7 %zu\n", m7 - allocator->memory);
    printf("HERE8 %zu\n", m8 - allocator->memory);
    printf("HERE9 %zu\n", m9 - allocator->memory);
    // printf("HERE %zu\n", allocBlock(allocator, 1) - allocator->memory);



    freeBlock(allocator, m2);
    freeBlock(allocator, m3);
    freeBlock(allocator, m7);
    printBlocks(allocator);

    void* m10 = allocBlock(allocator, 128 - 24);
    printf("HERE10 %zu\n", m10 - allocator->memory);
    printBlocks(allocator);
    freeBlock(allocator, m10);
    freeBlock(allocator, m8);
    freeBlock(allocator, m1);
    freeBlock(allocator, m4);
    printBlocks(allocator);
    printBlocks(allocator);
}