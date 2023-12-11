#include "../include/linkedList.h"

void push(BlockInfo** head, BlockInfo* new) {
    // if (*head != NULL) {
    //     (*head)->prev = new;
    // }
    new->next = *head;
    // new->prev = NULL;
    *head = new;
}

BlockInfo* pop(BlockInfo** head) {
    BlockInfo* result;
    result = *head;
    if ((*head)->next == NULL) {
        *head = NULL;
    } else {
        (*head) = (*head)->next;
        // (*head)->prev = NULL;
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