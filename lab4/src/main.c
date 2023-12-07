#include "../include/functions.h"
#include <stdio.h>

int main() {
    float a, b, e;
    int c, d;

    scanf("%f%f%f", &a, &b, &e);
    scanf("%d%d", &c, &d);
    printf("Rectangle result: %f\n", SinIntegralRectangleMethod(a, b, e));

    printf("Trapezoid result: %f\n", SinIntegralTrapezoidMethod(a, b, e));

    printf("GCF naive = %d\n", GCFNaive(d, c));
    printf("GCF Euclid = %d\n", GCFNaive(c, d));
}