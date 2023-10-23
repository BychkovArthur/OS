#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct threadData_t threadData_t;

typedef struct Task Task;

typedef struct threadData_t {
    int* arr;
    int* buff;
    int size;
    int threadCnt;
} threadData_t;

typedef struct Task {
    int* array;
    int* buffer;
    int size;
    int type;
    Task* otherTask;
} Task;

#endif // STRUCTURES_H