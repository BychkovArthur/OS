#ifndef LAB1_FUNCTIONS_H
#define LAB1_FUNCTIONS_H

int getNumberOfNumbers(char* stringOfNumbers, int stringLen);

void fillArrayWithNumbers(char *stringOfNumbers, int stringLen, int* array);

double devide(int* array, int numberOfNumbers, int* childProcessExitStatus);

void fileNameValidation(char* fileName);

#endif //LAB1_FUNCTIONS_H