#include "../include/myLib1.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/functions.h"

float SinIntegral(float A, float B, float e) { // Rectangle method
    isStepPositive(e);

    int isRightLimitGreater = 1;
    if (B < A) {
        isRightLimitGreater = 0;
        swapf(&A, &B);
    }

    float result = 0;
    for (float currentLeftDot = A; currentLeftDot < B; currentLeftDot += e) {
        float segmentLength = fminf(e, B - currentLeftDot);
        result += sinf(currentLeftDot + segmentLength / 2.0) * segmentLength;
    }

    return isRightLimitGreater ? result : -result;
}

int GCF(int a, int b) { // Euclid
    isNumbersNatural(a, b);

    while (1) {
        if (a > b) { // Предполагаем, что а всегда меньше b
            swapi(&a, &b);
        }

        if ((b = (b % a)) == 0) {
            return a;
        }
    }
}