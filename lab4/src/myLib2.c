#include "../include/myLib2.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/functions.h"

float SinIntegral(float A, float B, float e) { // Trapezoid method
    isStepPositive(e);

    int isRightLimitGreater = 1;
    if (B < A) {
        isRightLimitGreater = 0;
        swapf(&A, &B);
    }

    float result = 0;
    for (float currentLeftDot = A; currentLeftDot < B; currentLeftDot += e) {
        float segmentLength = fminf(e, B - currentLeftDot);
        float leftTrapezoidBorder = sinf(currentLeftDot);
        float rightTrapezoidBorder = sinf(currentLeftDot + segmentLength);
        result += (leftTrapezoidBorder + rightTrapezoidBorder) / 2 * segmentLength;
    }

    return isRightLimitGreater ? result : -result;
}

int GCF(int a, int b) { // Naive
    isNumbersNatural(a, b);

    for (int currentDivisior = min(a, b); currentDivisior >= 1; --currentDivisior) {
        if (a % currentDivisior == 0 && b % currentDivisior == 0) {
            return currentDivisior;
        }
    }
    return -1;
}