#include <stdio.h>

#include "../include/myLib1.h"
#include "../include/functions.h"

int main() {
    const int isRunTimeLinking = 0;
    int operationType = 0;
    
    readOperationType(&operationType, isRunTimeLinking);

    if (operationType == SWITCH_IMPLEMENTATION) {
        fprintf(stderr, "Invalid number");
        exit(1);
    }

    if (operationType == SIN_INTEGRAL) {
        float A, B, e;
        readFloatValue(">>> Введите нижний предел интегрирования:\n", &A);
        readFloatValue(">>> Введите верхний предел интегрирования:\n", &B);
        readFloatValue(">>> Введите шаг интегрирования:\n", &e);
        printf("Интеграл, вычисленный методом средних прямогольников равен: %f\n", SinIntegral(A, B, e));
    } else if (operationType == GCF_CALC) {
        int a, b;
        readIntValue(">>> Введите первое число:\n", &a);
        readIntValue(">>> Введите второе число:\n", &b);
        printf("НОД, вычисленный алгоритмом Евклида равен: %d\n", GCF(a, b));
    }

    return 0;
}