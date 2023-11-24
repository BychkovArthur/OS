#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stddef.h>

double devide(int *, int, int *);
int readFileName(char *);
int getNumberOfNumbers(char *, size_t);
void fillArrayWithNumbers(char *, size_t, int *);
void updateQuitStatus(char *, int, int *);

#endif  // FUNCTIONS_H