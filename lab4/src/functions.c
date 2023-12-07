#include "../include/functions.h"

#include <stdio.h>
#include <stdlib.h>

void readOperationType(int* operationType) {
    printf(">>> Введите тип операции (1) - Интеграл от синуса, (2) - Нахождение НОД:\n");
    printf("> ");
    if (scanf("%d", operationType) != 1 || !(*operationType >= SIN_INTEGRAL && *operationType <= GCF_CALC)) {
        fprintf(stderr, "Invalid number");
        exit(1);
    }
}

void readFloatValue(const char* messageString, float* variable) {
    printf(messageString);
    printf("> ");
    if (scanf("%f", variable) != 1) {
        fprintf(stderr, "Invalid number");
        exit(1);
    }
}

void readIntValue(const char* messageString, int* variable) {
    printf(messageString);
    printf("> ");
    if (scanf("%d", variable) != 1) {
        fprintf(stderr, "Invalid number");
        exit(1);
    }
}

void swapf(float* a, float* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void swapi(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int min(int a, int b) {
    return a < b ? a : b;
}

void isStepPositive(float step) {
    if (step <= 0) {
        fprintf(stderr, "(ERROR) Step must be positive");
        exit(1);
    }
}

void isNumberNatural(int a) {
    if (a <= 0) {
        fprintf(stderr, "(ERROR) Number (%d) aren't natural\n", a);
        exit(1);
    }
}

void isNumbersNatural(int a, int b) {
    isNumberNatural(a);
    isNumberNatural(b);
}