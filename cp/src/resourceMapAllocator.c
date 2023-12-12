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
        memorySize - ALIGN_BY,
    };
    BlockInfo* firstBlock = (BlockInfo*)allocator->memory;
    *firstBlock = block;

    firstBlock->nextBlock = setBlockFree(firstBlock);
    printf("Total allocated: %zu\n", memorySize);
    printf("IN START %p\n", firstBlock->nextBlock);
    allocator->firstFreeBLock = firstBlock;
    return allocator;
}

// size_t getBlockLengthByBlock(Allocator* allocator, BlockInfo* blockInfo) {
//     BlockInfo* nextBlock = resetToNormalPointer(blockInfo->nextBlock);
//     if (nextBlock == NULL) {
//         return allocator->memorySize - ((uint8_t*)blockInfo - (uint8_t*)allocator->memory) - ALIGN_BY;
//     }
//     return ((uint8_t*)nextBlock - (uint8_t*)blockInfo) - ALIGN_BY;
// }

size_t getBlockLengthByGivenMemory(Allocator* allocator, void* memory) {
    return ((BlockInfo*)((uint8_t*)memory - ALIGN_BY))->blockSize;
}

// Сдвиг начала блока относительно начала выделенной памяти
size_t getOffset(Allocator* allocator, BlockInfo* block) {
    return (size_t)((uint8_t*)block - (uint8_t*)allocator->memory);
}

void* allocBlock(Allocator* allocator, size_t blockSize) {
    blockSize = align(blockSize);
    BlockInfo* currentBlock = allocator->firstFreeBLock;
    printf("%p------------------------\n", currentBlock);
    // printf("228 : IN START %p\n", currentBlock->nextBlock);
    BlockInfo* prevBlock = NULL;
    BlockInfo* bestBlock = NULL;
    BlockInfo* bestPrevBlock = NULL;
    size_t minLength = -1;
    
    while (currentBlock != NULL) {
        printf("offset: %zu size %zu nextOffset: %p\n", getOffset(allocator, currentBlock), currentBlock->blockSize, currentBlock->nextBlock);
        // Если можно объединить, объединяем
        // Но, чтобы обработать ситуацию когда 3 и более подряд пустых, мы после склеивания не должны менять текущий.
        if (adjacentBlocksExistsAndFree(currentBlock)) {
            currentBlock->blockSize += (ALIGN_BY + resetToNormalPointer(currentBlock->nextBlock)->blockSize);
            currentBlock->nextBlock = resetToNormalPointer(currentBlock->nextBlock)->nextBlock;
            continue;
        }
        // size_t currentBlockLength = getBlockLengthByBlock(allocator, currentBlock);
        size_t currentBlockLength = currentBlock->blockSize;
        printf("SIZE = %zu\n", currentBlockLength);
        if (currentBlockBetter(currentBlock, currentBlockLength, blockSize, minLength)) {
            printf("HERE-----------------\n");
            bestPrevBlock = prevBlock;
            bestBlock = currentBlock;
            minLength = currentBlockLength;
        }
        prevBlock = currentBlock;
        currentBlock = resetToNormalPointer(currentBlock->nextBlock);
    }

    if (bestBlock == NULL) {
        fprintf(stderr, "Can't allocate new memory\n");
        exit(1);
    }
    
    if (bestBlock == allocator->firstFreeBLock) {
        // allocator->firstFreeBLock = bestBlock->nextBlock;
        
        if (bestBlock->blockSize >= blockSize + ALIGN_BY) {
            blockSize += ALIGN_BY;
            BlockInfo* newBlock = (BlockInfo*)((uint8_t*)bestBlock + blockSize);
            // Для нового блока следующий свободный - следующий для лучшего
            newBlock->nextBlock = resetToNormalPointer(bestBlock->nextBlock);
            newBlock->nextBlock = setBlockFree(newBlock);

            newBlock->blockSize = bestBlock->blockSize - blockSize;

            // bestBlock->nextBlock = newBlock; // Вот это под вопросом
            bestBlock->nextBlock = setBlockOccupied(bestBlock);
            bestBlock->blockSize = blockSize - ALIGN_BY;

            allocator->firstFreeBLock = newBlock;
        } else {
            
            allocator->firstFreeBLock = resetToNormalPointer(bestBlock->nextBlock);
            bestBlock->nextBlock = setBlockOccupied(bestBlock);
            bestBlock->blockSize = blockSize;
            
        }
    } else {

        if (bestBlock->blockSize >= blockSize + ALIGN_BY) {
            blockSize += ALIGN_BY;
            BlockInfo* newBlock = (BlockInfo*)((uint8_t*)bestBlock + blockSize);
            // Для нового блока следующий свободный - следующий для лучшего
            newBlock->nextBlock = resetToNormalPointer(bestBlock->nextBlock);
            newBlock->nextBlock = setBlockFree(newBlock);

            
            // bestBlock->nextBlock = newBlock; // Вот это под вопросом
            bestBlock->nextBlock = setBlockOccupied(bestBlock);
            bestBlock->blockSize = blockSize - ALIGN_BY;

            bestPrevBlock->nextBlock = newBlock;
        } else {
            bestPrevBlock->nextBlock = resetToNormalPointer(bestBlock->nextBlock);
            bestBlock->nextBlock = setBlockOccupied(bestBlock);
            bestBlock->blockSize = blockSize;
        }
    }




    // Если можно расположить следующий blockInfo, делаем
    // if (getBlockLengthByBlock(allocator, bestBlock) >= blockSize + ALIGN_BY) {
    //     blockSize += ALIGN_BY;
    //     BlockInfo* newBlock = (BlockInfo*)((uint8_t*)bestBlock + blockSize);
    //     newBlock->nextBlock = resetToNormalPointer(bestBlock->nextBlock);
    //     newBlock->nextBlock = setBlockFree(newBlock);
    //     bestBlock->nextBlock = newBlock;
    //     bestBlock->nextBlock = setBlockOccupied(bestBlock);
    // } else {
    //     bestBlock->nextBlock = setBlockOccupied(bestBlock);
    // }

    printf("laksjdflaksdjf\n");
    return (void*)(((uint8_t*)bestBlock) + ALIGN_BY);
}

void freeBlock(Allocator* allocator, void* memoryBlock) {
    BlockInfo* blockForFree = (BlockInfo*)((uint8_t*)memoryBlock - ALIGN_BY);
    if (!(allocator->memory <= memoryBlock && memoryBlock <= (void*)((uint8_t*)allocator->memory + allocator->memorySize)) || isBlockFree(blockForFree)) {
        fprintf(stderr, "Can't free this block\n");
        exit(1);
    }
    // currentBlock->nextBlock = setBlockFree(currentBlock);
    BlockInfo* currentBlock = allocator->firstFreeBLock;

    if (currentBlock == blockForFree) {
        fprintf(stderr, "Something went wrong ...\n");
        exit(1);
    }

    if (blockForFree < currentBlock || currentBlock == NULL) {
        // printf("currentblock == null\n");
        blockForFree->nextBlock = currentBlock;
        blockForFree->nextBlock = setBlockFree(blockForFree);
        allocator->firstFreeBLock = blockForFree;
        allocator->firstFreeBLock->nextBlock = setBlockFree(allocator->firstFreeBLock);
    } else {
        while (resetToNormalPointer(currentBlock->nextBlock) != NULL && resetToNormalPointer(currentBlock->nextBlock) < blockForFree) {
            currentBlock = resetToNormalPointer(currentBlock->nextBlock);
        }
        blockForFree->nextBlock = resetToNormalPointer(currentBlock->nextBlock);
        blockForFree->nextBlock = setBlockFree(blockForFree);
        currentBlock->nextBlock = blockForFree;
        currentBlock->nextBlock = setBlockFree(currentBlock);
    }
}

bool adjacentBlocksExistsAndFree(BlockInfo* currentBlock) {
    return resetToNormalPointer(currentBlock->nextBlock) == (BlockInfo*)((uint8_t*)currentBlock + ALIGN_BY + currentBlock->blockSize) && isBlockFree(currentBlock) && isBlockFree(resetToNormalPointer(currentBlock->nextBlock));
}

bool currentBlockBetter(BlockInfo* currentBlock, size_t currentBlockLength, size_t blockSize, size_t minLength) {
    return isBlockFree(currentBlock) && currentBlockLength >= blockSize && currentBlockLength < minLength;
}

void destroyMemoryAllocator(Allocator* allocator) {
    free(allocator->memory);
    free(allocator);
}