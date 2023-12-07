#include <stdio.h>

#include "../include/functions.h"

int main() {
    const int isRunTimeLinking = 1;
    int currentImpementationOfFunctions = 1;
    int operationType = -1;
    void* handleToLib1 = NULL;
    void* handleToLib2 = NULL;
    initRunTimeLibrary(&handleToLib1, &handleToLib2);

    float (*SinIntegral)(float, float, float) = NULL;
    int (*GCF)(int, int) = NULL;

    updateFuncions(handleToLib1, handleToLib2, &SinIntegral, &GCF, currentImpementationOfFunctions);
    
    while (1)
    {
        printf("Сейчас выбрана библиотека MyLib%d.so\n", currentImpementationOfFunctions);
        readOperationType(&operationType, isRunTimeLinking);
        if (operationType == SWITCH_IMPLEMENTATION) {
            currentImpementationOfFunctions = ((currentImpementationOfFunctions == 1) ? 2 : 1);
            updateFuncions(handleToLib1, handleToLib2, &SinIntegral, &GCF, currentImpementationOfFunctions);
        } else if (operationType == SIN_INTEGRAL) {
            float A, B, e;
            readFloatValue(">>> Введите нижний предел интегрирования:\n", &A);
            readFloatValue(">>> Введите верхний предел интегрирования:\n", &B);
            readFloatValue(">>> Введите шаг интегрирования:\n", &e);
            if (currentImpementationOfFunctions == 1) {
                printf("Интеграл, вычисленный методом средних прямогольников равен: %f\n", SinIntegral(A, B, e));
            } else {
                printf("Интеграл, вычисленный методом трапеций равен: %f\n", SinIntegral(A, B, e));
            }
        } else if (operationType == GCF_CALC) {
            int a, b;
            readIntValue(">>> Введите первое число:\n", &a);
            readIntValue(">>> Введите второе число:\n", &b);
            if (currentImpementationOfFunctions == 1) {
                printf("НОД, вычисленный алгоритмом Евклида равен: %d\n", GCF(a, b));
            } else {
                printf("НОД, вычисленный наивным алгоритмом равен: %d\n", GCF(a, b));
            }
        }
    }
    

    return 0;
}