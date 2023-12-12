#include "../include/buddySystemLinkedList.h"

void push(BlockInfo** head, BlockInfo* new) {
    new->next = *head;
    *head = new;
}

BlockInfo* pop(BlockInfo** head) {
    BlockInfo* result;
    result = *head;
    if ((*head)->next == NULL) {
        *head = NULL;
    } else {
        (*head) = (*head)->next;
        result->next = NULL;
    }
    return result;
}

void removeBlock(BlockInfo** head, BlockInfo* target) {
    if ((*head) == target) {
        *head = (*head)->next;
        return;
    }
    BlockInfo* currentBlock = *head;
    while (currentBlock->next != target) {
        currentBlock = currentBlock->next;
    }
    currentBlock->next = target->next;
    
}