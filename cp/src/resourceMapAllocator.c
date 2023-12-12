#include "../include/resourceMapAllocator.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "../include/resourceMapBlockInfo.h"

size_t align(size_t memorySize) {
    return memorySize + (ALIGN_BY - (memorySize % ALIGN_BY)) % ALIGN_BY;
}

Allocator* createMemoryAllocator(size_t memorySize) {
    if (memorySize == 0) {
        fprintf(stderr, "Invalid memory size 0\n");
        exit(1);
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
    BlockInfo* firstBlock = (BlockInfo*)allocator->memory;
    *firstBlock = block;

    firstBlock->nextBlock = setBlockFree(firstBlock);
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

    blockSize = align(blockSize);
    BlockInfo* currentBlock = (BlockInfo*)allocator->memory;
    BlockInfo* bestBlock = NULL;
    size_t minLength = -1;

    while (currentBlock != NULL) {
        // Если можно объединить, объединяем
        // Но, чтобы обработать ситуацию когда 3 и более подряд пустых, мы после склеивания не должны менять текущий.
        if (adjacentBlocksExistsAndFree(currentBlock)) {
            currentBlock->nextBlock = resetToNormalPointer(currentBlock->nextBlock)->nextBlock;
            continue;
        }
        size_t currentBlockLength = getBlockLengthByBlock(allocator, currentBlock);
        if (currentBlockBetter(currentBlock, currentBlockLength, blockSize, minLength)) {
            bestBlock = currentBlock;
            minLength = currentBlockLength;
        }
        currentBlock = resetToNormalPointer(currentBlock->nextBlock);
    }

    if (bestBlock == NULL) {
        fprintf(stderr, "Can't allocate new memory\n");
        exit(1);
    }

    // Если можно расположить следующий blockInfo, делаем
    if (getBlockLengthByBlock(allocator, bestBlock) >= blockSize + ALIGN_BY) {
        blockSize += ALIGN_BY;
        BlockInfo* newBlock = (BlockInfo*)((uint8_t*)bestBlock + blockSize);
        newBlock->nextBlock = resetToNormalPointer(bestBlock->nextBlock);
        newBlock->nextBlock = setBlockFree(newBlock);
        bestBlock->nextBlock = newBlock;
        bestBlock->nextBlock = setBlockOccupied(bestBlock);
    } else {
        bestBlock->nextBlock = setBlockOccupied(bestBlock);
    }
    return (void*)(((uint8_t*)bestBlock) + ALIGN_BY);
}

void freeBlock(const Allocator* allocator, void* memoryBlock) {
    BlockInfo* currentBlock = (BlockInfo*)((uint8_t*)memoryBlock - ALIGN_BY);
    if (!(allocator->memory <= memoryBlock && memoryBlock <= (void*)((uint8_t*)allocator->memory + allocator->memorySize)) || isBlockFree(currentBlock)) {
        fprintf(stderr, "Can't free this block\n");
        exit(1);
    }
    currentBlock->nextBlock = setBlockFree(currentBlock);
}

bool adjacentBlocksExistsAndFree(BlockInfo* currentBlock) {
    return resetToNormalPointer(currentBlock->nextBlock) != NULL && isBlockFree(currentBlock) && isBlockFree(resetToNormalPointer(currentBlock->nextBlock));
}

bool currentBlockBetter(BlockInfo* currentBlock, size_t currentBlockLength, size_t blockSize, size_t minLength) {
    return isBlockFree(currentBlock) && currentBlockLength >= blockSize && currentBlockLength < minLength;
}

void destroyMemoryAllocator(Allocator* allocator) {
    free(allocator->memory);
    free(allocator);
}