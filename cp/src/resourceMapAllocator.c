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
    *(BlockInfo*)allocator->memory = block;

    allocator->firstBlock = setBlockFree((BlockInfo*)allocator->memory);
    return allocator;
}

size_t getBlockLength(Allocator* allocator, BlockInfo* blockInfo) {
    if (blockInfo->nextBlock == NULL) {
        return allocator->memorySize - ((uint8_t*)blockInfo - (uint8_t*)allocator->memory) - ALIGN_BY;
    }

    return ((uint8_t*)resetToNormalPointer(blockInfo->nextBlock) - (uint8_t*)blockInfo) - ALIGN_BY;
}

void* allocBlock(Allocator* allocator, size_t blockSize) {

    blockSize += ALIGN_BY;
    blockSize = align(blockSize);
    BlockInfo* currentBlock = resetToNormalPointer(allocator->firstBlock);

    // Если всего один блок
    if (currentBlock->nextBlock == NULL) {
        allocator->firstBlock = setBlockOccupied(currentBlock);
        currentBlock->nextBlock = (BlockInfo*)((uint8_t*)currentBlock + blockSize);
        currentBlock->nextBlock->nextBlock = NULL;
        currentBlock->nextBlock = setBlockFree(currentBlock->nextBlock);
        return (void*)((uint8_t)currentBlock->nextBlock + ALIGN_BY);
    }

    BlockInfo* bestBlock = NULL;
    size_t minLength = -1;

    while (currentBlock != NULL) {
        size_t currentBlockLength = getBlockLength(allocator, currentBlock);
        if (currentBlock->isFree && currentBlockLength >= blockSize && currentBlockLength < minLength) {
            bestBlock = resetToNormalPointer(currentBlock);
            minLength = currentBlockLength;
        }
        currentBlock = resetToNormalPointer(currentBlock->nextBlock);
    }

    BlockInfo* newBlock = (BlockInfo*)((uint8_t*)bestBlock + blockSize);
    newBlock->nextBlock = bestBlock->nextBlock;
    newBlock->isFree = true;
    bestBlock->nextBlock = newBlock;
    bestBlock->isFree = false;
    return ((uint8_t*)bestBlock) + ALIGN_BY;
}

void* freeBlock(Allocator* allocator, void* block);