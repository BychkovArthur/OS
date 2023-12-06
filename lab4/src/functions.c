#include "../include/functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

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

float SinIntegralRectangleMethod(float A, float B, float e) {
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

float SinIntegralTrapezoidMethod(float A, float B, float e) {
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

int GCFEuclid(int a, int b) {
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

int GCFNaive(int a, int b) {
    isNumbersNatural(a, b);

    for (int currentDivisior = min(a, b); currentDivisior >= 1; --currentDivisior) {
        if (a % currentDivisior == 0 && b % currentDivisior == 0) {
            return currentDivisior;
        }
    }    
}

// int main() {
//     float a, b, e;
//     int c, d;

//     scanf("%f%f%f", &a, &b, &e);
//     scanf("%d%d", &c, &d);
//     printf("Rectangle result: %f\n", SinIntegralRectangleMethod(a, b, e));

//     printf("Trapezoid result: %f\n", SinIntegralTrapezoidMethod(a, b, e));

//     printf("GCF naive = %d\n", GCFNaive(d, c));
//     printf("GCF Euclid = %d\n", GCFNaive(c, d));
// }