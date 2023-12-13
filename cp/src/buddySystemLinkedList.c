#include "../include/buddySystemLinkedList.h"

#include <stdio.h>

void push(BlockInfo** head, BlockInfo* new) {
    new->next = *head;
    new->prev = NULL;

    if (*head != NULL) {
        (*head)->prev = new;
    }

    *head = new;

}

BlockInfo* pop(BlockInfo** head) {
    BlockInfo* result;
    result = *head;
    if ((*head)->next == NULL) {
        *head = NULL;
    } else {
        (*head)->next->prev = NULL;
        (*head) = (*head)->next;
        result->next = NULL;
    }
    return result;
}

void removeBlock(BlockInfo** head, BlockInfo* target) {
    // Удаление с начала
    if ((*head) == target) {
        // printf("HERE remove\n");
        BlockInfo* tmp = (*head);
        *head = (*head)->next;
        if (*head != NULL) {
            (*head)->prev = NULL;
        }
        tmp->next = NULL;
        // printf("HERE remove end\n");
        return;
    }
    // Удаление с конца
    if (target->next == NULL) {
        // printf("here---\n");
        target->prev->next = NULL;
        target->prev = NULL;
        // printf("here--2-\n");
        return;
    }
    // printf("ABOBA---\n");
    BlockInfo* currentBlock = target;
    currentBlock->prev->next = currentBlock->next;
    currentBlock->next->prev = currentBlock->prev;
    currentBlock->next = NULL;
    currentBlock->prev = NULL;
    // printf("ABOBA---2\n");
    // while (currentBlock->next != target) {
    //     currentBlock = currentBlock->next;
    // }
    // currentBlock->next = target->next;    
}