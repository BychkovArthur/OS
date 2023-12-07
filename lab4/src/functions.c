#include "../include/functions.h"

#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <unistd.h>
#include <libgen.h>
#include <string.h>
#include <dlfcn.h>

#define LIB1_NAME "libMyLib1.so"
#define LIB2_NAME "libMyLib2.so"
#define INTEGRAL_FUNCTION_NAME "SinIntegral"
#define GCF_FUNCTION_NAME "GCF"

void initRunTimeLibrary(void** handleToLib1, void** handleToLib2) {
    char pathName[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", pathName, sizeof(pathName) - 1);

    if (count == -1) {
        perror("Can't get sym link on binary file");
        exit(1);
    }

    pathName[count] = '\0';
    dirname(pathName);
    strcat(pathName, "/");

    char pathToLib1[PATH_MAX];
    char pathToLib2[PATH_MAX];
    size_t pathLen = strlen(pathName);

    if (pathLen + strlen(LIB1_NAME) + 1 > PATH_MAX) {
        fprintf(stderr, "Invalid path lengnt to %s\n", LIB1_NAME);
    }
    if (pathLen + strlen(LIB2_NAME) + 1 > PATH_MAX) {
        fprintf(stderr, "Invalid path lengnt to %s\n", LIB2_NAME);
    }
    strcpy(pathToLib1, pathName);
    strcpy(pathToLib2, pathName);
    strcat(pathToLib1, LIB1_NAME);
    strcat(pathToLib2, LIB2_NAME);
    
    *handleToLib1 = dlopen(pathToLib1, RTLD_LAZY);
    *handleToLib2 = dlopen(pathToLib2, RTLD_LAZY);

    if (*handleToLib1 == NULL || *handleToLib2 == NULL) {
        perror("Error while dlopen");
        exit(1);
    }
}

void updateFuncions(void* handleToLib1, void* handleToLib2, float (**SinIntegral)(float, float, float), int (**GCF)(int, int), int currentImpementationOfFunctions) {
    if (currentImpementationOfFunctions == 1) {
        *SinIntegral = (float (*)(float, float, float))dlsym(handleToLib1, INTEGRAL_FUNCTION_NAME);
        *GCF = (int (*)(int, int))dlsym(handleToLib1, GCF_FUNCTION_NAME);
    } else if (currentImpementationOfFunctions == 2) {
        *SinIntegral = (float (*)(float, float, float))dlsym(handleToLib2, INTEGRAL_FUNCTION_NAME);
        *GCF = (int (*)(int, int))dlsym(handleToLib2, GCF_FUNCTION_NAME);
    }
    if (SinIntegral == NULL || GCF == NULL) {
        perror("Error while dlsym");
        exit(1);
    }
}

void readOperationType(int* operationType, const int isRunTimeLinking) {
    if (isRunTimeLinking) {
        printf(">>> Введите тип операции: (0) - Сменить текущую реализацию, (1) - Интеграл от синуса, (2) - Нахождение НОД, (3) - Выход:\n");
    } else {
        printf(">>> Введите тип операции: (1) - Интеграл от синуса, (2) - Нахождение НОД, (3) - Выход:\n");
    }
    printf("> ");
    if (scanf("%d", operationType) != 1 || !(*operationType >= SWITCH_IMPLEMENTATION && *operationType <= EXIT)) {
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