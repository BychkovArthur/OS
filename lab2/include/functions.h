#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "structures.h"

int isArrayStrictlyIncreasing(int*, int);
int isArraysStrictlyIncreasing(int**, int, int);
void usage();
int strToInt(char*);
void createRandomTasks(int**, int**, Task*, int, int, int, int*);
void executeTask(Task*, int);
void finishTask(int*, int*, Task*, int);

#endif // FUNCTIONS_H