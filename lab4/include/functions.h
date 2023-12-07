#ifndef __FUNCTIONS__
#define __FUNCTIONS__

enum OperationType {
    SWITCH_IMPLEMENTATION,
    SIN_INTEGRAL,
    GCF_CALC,
};

void initRunTimeLibrary(void**, void**);
void updateFuncions(void*, void*, float (**)(float, float, float), int (**)(int, int), int);

void readOperationType(int*, const int);
void readFloatValue(const char*, float*);
void readIntValue(const char*, int*);

void swapf(float*, float*);
void swapi(int*, int*);

int min(int, int);

void isStepPositive(float);

void isNumberNatural(int);
void isNumbersNatural(int, int);

#endif // __FUNCTIONS__