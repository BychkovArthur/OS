#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "structures.h"

int isArrayStrictlyIncreasing(int*, int);
int isArraysStrictlyIncreasing(int**, int, int);
void usage();
int strToInt(char*);
void createRandomTasks(int**, int**, regularTask_t*, finishTask_t*, int, int, int, int*, int*);
void executeRegularTask(regularTask_t*, int);
void executeFinishTask(finishTask_t*, int);
void finishTask(int*, int*, regularTask_t*, int);

#endif // FUNCTIONS_H