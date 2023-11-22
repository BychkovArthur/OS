#ifndef STRUCTURES_H
#define STRUCTURES_H

typedef struct threadData_t threadData_t;

typedef struct regularTask_t regularTask_t;

typedef struct finishTask_t finishTask_t;

typedef struct threadData_t {
    int* arr;
    int* buff;
    int size;
    int threadCnt;
} threadData_t;

typedef struct regularTask_t {
    int* array;
    int* buffer;
    int size;
} regularTask_t;

typedef struct finishTask_t {
    int* array;
    int* buffer;
    regularTask_t* regularTask;
} finishTask_t;

#endif // STRUCTURES_H