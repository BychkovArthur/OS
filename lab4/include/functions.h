#ifndef __FUNCTIONS__
#define __FUNCTIONS__

enum OperationType {
    SIN_INTEGRAL = 1,
    GCF_CALC,
};

void readOperationType(int*);
void readFloatValue(const char*, float*);
void readIntValue(const char*, int*);

void swapf(float*, float*);
void swapi(int*, int*);

int min(int, int);

void isStepPositive(float);

void isNumberNatural(int);
void isNumbersNatural(int, int);

#endif // __FUNCTIONS__