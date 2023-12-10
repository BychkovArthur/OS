#include "../include/resourceMapAllocator.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "../include/blockInfo.h"

size_t align(size_t memorySize) {
    return memorySize + (ALIGN_BY - (memorySize % ALIGN_BY)) % ALIGN_BY;
}

Allocator* createMemoryAllocator(size_t memorySize) {
    if (memorySize == 0) {
        fprintf(stderr, "Invalid memory size 0\n");
    }

    Allocator* allocator = malloc(sizeof(Allocator));
    if (allocator == NULL) {
        perror("Can't create allocator");
        exit(1);
    }

    // Добавляем размер структуры
    memorySize += ALIGN_BY;
    memorySize = align(memorySize); // Выравниваем саму память
    printf("MEMORY SIZE: %zu\n", memorySize);
    uint8_t* memory = malloc(memorySize); // malloc выравнивает память по 8 (или 16 или 32...)
    if (memory == NULL) {
        fprintf(stderr, "Can't allocate memory\n");
        exit(1);
    }

    allocator->memory = (void*)memory;
    allocator->memorySize = memorySize;

    // В начало ставим блок, который указывает на всю память
    BlockInfo block = {
        NULL,
    };
    BlockInfo* firstBlock = (BlockInfo*)allocator->memory;
    *firstBlock = block;

    firstBlock->nextBlock = setBlockFree(firstBlock->nextBlock);
    return allocator;
}

size_t getBlockLengthByBlock(Allocator* allocator, BlockInfo* blockInfo) {
    BlockInfo* nextBlock = resetToNormalPointer(blockInfo->nextBlock);
    if (nextBlock == NULL) {
        return allocator->memorySize - ((uint8_t*)blockInfo - (uint8_t*)allocator->memory) - ALIGN_BY;
    }
    return ((uint8_t*)nextBlock - (uint8_t*)blockInfo) - ALIGN_BY;
}

size_t getBlockLengthByGivenMemory(Allocator* allocator, void* memory) {
    return getBlockLengthByBlock(allocator, (BlockInfo*)((uint8_t*)memory - ALIGN_BY));
}

void* allocBlock(Allocator* allocator, size_t blockSize) {

    // blockSize += ALIGN_BY;
    blockSize = align(blockSize);
    BlockInfo* currentBlock = (BlockInfo*)allocator->memory;
    BlockInfo* bestBlock = NULL;
    size_t minLength = -1;

    while (currentBlock != NULL) {
        size_t currentBlockLength = getBlockLengthByBlock(allocator, currentBlock);
        if (isBlockFree(currentBlock->nextBlock) && currentBlockLength >= blockSize && currentBlockLength < minLength) {
            bestBlock = currentBlock;
            minLength = currentBlockLength;
        }
        currentBlock = resetToNormalPointer(currentBlock->nextBlock);
    }

    if (bestBlock == NULL) {
        fprintf(stderr, "Can't allocate new memory\n");
        exit(1);
    }

    // Если можно расположить следующий пункт информации, делаем
    if (getBlockLengthByBlock(allocator, bestBlock) >= blockSize + ALIGN_BY) {
        blockSize += ALIGN_BY;
        BlockInfo* newBlock = (BlockInfo*)((uint8_t*)bestBlock + blockSize);
        newBlock->nextBlock = resetToNormalPointer(bestBlock->nextBlock);
        newBlock->nextBlock = setBlockFree(newBlock->nextBlock);
        bestBlock->nextBlock = setBlockOccupied(newBlock);
    } else {
        bestBlock->nextBlock = setBlockOccupied(bestBlock->nextBlock);
    }
    return (void*)(((uint8_t*)bestBlock) + ALIGN_BY);
}

void* freeBlock(Allocator* allocator, void* memoryBlock) {

}