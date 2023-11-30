#define _GNU_SOURCE
#include <fcntl.h>
#include <libgen.h>  // для dirname
#include <limits.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../include/definitions.h"
#include "../include/functions.h"

unsigned char* createErrorAndExitStatusSHM(int* sharedMemoryFDErr) {
    unsigned char* errorsAndExitStatus = NULL;
    *sharedMemoryFDErr = shm_open(SHARED_MEMORY_NAME_ERR, O_CREAT | O_RDWR, S_IRWXU);

    if (*sharedMemoryFDErr == -1) {
        perror("shm_open (parent)");
        exit(1);
    }

    // Обрезаем длину памяти
    if (ftruncate(*sharedMemoryFDErr, ERROR_SHM_SIZE) == -1) {
        perror("ftruncate (parent)");
        exit(1);
    }

    if ((errorsAndExitStatus = mmap(NULL, ERROR_SHM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED,
                                    *sharedMemoryFDErr, 0)) == MAP_FAILED) {
        perror("mmap (parent)");
        exit(1);
    }
    errorsAndExitStatus[0] = NOTHING;
    return errorsAndExitStatus;
}

void reopenSemaphore(sem_t** semaphoreChild, sem_t** semaphoreParent) {
    if ((*semaphoreParent = sem_open(SEMAPHORE_FOR_SHM_1, O_CREAT, S_IRWXU, 1)) == SEM_FAILED ||
        (*semaphoreChild = sem_open(SEMAPHORE_FOR_SHM_2, O_CREAT, S_IRWXU, 0)) == SEM_FAILED) {
        perror("semaphore (parent)");
        exit(1);
    }

    // Если семафор не закрылся в прошлый раз - закрываем все!
    if (sem_close(*semaphoreParent) == -1 || sem_unlink(SEMAPHORE_FOR_SHM_1) == -1 ||
        sem_close(*semaphoreChild) == -1 || sem_unlink(SEMAPHORE_FOR_SHM_2) == -1) {
        perror("semaphore close/unlink (parent)");
        exit(1);
    }

    if ((*semaphoreParent = sem_open(SEMAPHORE_FOR_SHM_1, O_CREAT, S_IRWXU, 1)) == SEM_FAILED ||
        (*semaphoreChild = sem_open(SEMAPHORE_FOR_SHM_2, O_CREAT, S_IRWXU, 0)) == SEM_FAILED) {
        perror("semaphore (parent)");
        exit(1);
    }
}

void freeAllResources(sem_t* semaphoreChild, sem_t* semaphoreParent) {
    if (shm_unlink(SHARED_MEMORY_NAME) == -1) {
        perror("shm unlink (parent)");
        exit(1);
    }

    if (shm_unlink(SHARED_MEMORY_NAME_ERR) == -1) {
        perror("shm_err unlink (parent)");
        exit(1);
    }

    if (sem_unlink(SEMAPHORE_FOR_SHM_1) == -1 || sem_close(semaphoreParent) == -1 ||
        sem_unlink(SEMAPHORE_FOR_SHM_2) == -1 || sem_close(semaphoreChild) == -1) {
        perror("semaphore close/unlink (parent)");
        exit(1);
    }
}

void createChildProcess(sem_t* semaphoreChild, sem_t* semaphoreParent, int sharedMemoryFD,
                        int sharedMemoryFDErr, const char* fileName) {
    char pathName[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", pathName, sizeof(pathName) - 1);

    if (count != -1) {
        pathName[count] = '\0';  // добавляем завершающий нуль
        // используем dirname для извлечения директории из пути
        dirname(pathName);
    } else {
        fprintf(stderr, "path to executable file (parent)");
        exit(1);
    }
    if (strlen(pathName) + strlen("/child_exe") >= PATH_MAX - 1) {
        fprintf(stderr, "path to executable file (parent)");
        exit(1);
    }
    strcat(pathName, "/child_exe");

    if (dup2(sharedMemoryFD, STDIN_FILENO) == -1) {
        perror("dup2 stdin (child)");
        freeAllResources(semaphoreChild, semaphoreParent);
        exit(1);
    }
    if (dup2(sharedMemoryFDErr, STDOUT_FILENO) == -1) {
        perror("dup2 stdout (child)");
        freeAllResources(semaphoreChild, semaphoreParent);
        exit(1);
    }
    if (execl(pathName, pathName, fileName, NULL) == -1) {
        perror("exec error (child)");
        freeAllResources(semaphoreChild, semaphoreParent);
        exit(1);
    }
}

/* СРОСИТЬ:

 1) У меня была ошибка, когда я файловый дескриптор передавал как простое числа в аргументы командной строки.
 После, делал mmap и у меня был bad file descriptor, хотя число верное, я принтами проверил (в родителе и
 ребенке одно и то же) Но, когда я просто сделал dup2 все стало работать хорошо.

 2) Чтобы пути не зависили от WD норм ли просто находить полный путь до бинарника и обрезать до папки, где он
 лежит?
*/

int main() {
    // semParent должен останавливать/возобнавлять работу родителя
    // semChild должен останавливать/возобнавлять работу ребенка
    sem_t* semaphoreParent;
    sem_t* semaphoreChild;
    int sharedMemoryFDErr;
    int sharedMemoryFD;

    char fileName[NAME_MAX + 1];

    unsigned char* errorsAndExitStatus;

    pid_t pid;
    int qStatus = 0;

    // Creating general memory
    if ((sharedMemoryFD = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, S_IRWXU)) == -1) {
        perror("shm_open_general (parent)");
        exit(1);
    }

    // Creating error and exit status SHM
    errorsAndExitStatus = createErrorAndExitStatusSHM(&sharedMemoryFDErr);

    reopenSemaphore(&semaphoreChild, &semaphoreParent);

    printf("Введите имя файла: ");

    if (readFileName(fileName) == ERROR) {
        fprintf(stderr, "Invalid file name %s (parent)\n", fileName);
        freeAllResources(semaphoreChild, semaphoreParent);
        exit(1);
    }

    if ((pid = fork()) == -1) {
        perror("fork (parent)");
        freeAllResources(semaphoreChild, semaphoreParent);
        exit(1);
    }

    if (pid == 0) {  // Child process
        createChildProcess(semaphoreChild, semaphoreParent, sharedMemoryFD, sharedMemoryFDErr, fileName);
    }

    // Parent process
    while (1) {
        if (sem_wait(semaphoreParent) == -1) {
            perror("sem_wait (parent)");
            freeAllResources(semaphoreChild, semaphoreParent);
            exit(1);
        }

        // Была ошибка в дочернем процессе
        if (errorsAndExitStatus[0] == CHILD_PROCESS_ERROR_QUIT) {
            break;
        }

        char* inputString = NULL;
        size_t n = 0;
        ssize_t charactersReaded = 0;

        printf("Введите числа или q для выхода:\n");

        if ((charactersReaded = getline(&inputString, &n, stdin)) ==
            -1) {  // Считываем входную строку с переносом
            perror("GetLine error (parent)");
            freeAllResources(semaphoreChild, semaphoreParent);
            free(inputString);
            exit(1);
        }

        updateQuitStatus(inputString, charactersReaded, &qStatus);
        if (qStatus) {
            free(inputString);
            errorsAndExitStatus[0] = GENERAL_QUIT;
            if (sem_post(semaphoreChild) == -1) {
                perror("sem_post (parent)");
                freeAllResources(semaphoreChild, semaphoreParent);
                exit(1);
            }
            break;
        }

        int numberOfNumbers = getNumberOfNumbers(inputString, charactersReaded);
        if (numberOfNumbers <= 0) {
            perror("Invalid inputString (parent)");
            freeAllResources(semaphoreChild, semaphoreParent);
            free(inputString);
            exit(1);
        }

        int* arrayOfNumbers = malloc(sizeof(int) * numberOfNumbers);
        int* arrayInSharedMemory = NULL;
        if (arrayOfNumbers == NULL) {
            perror("Unable allocate buffer (parent)");
            freeAllResources(semaphoreChild, semaphoreParent);
            free(inputString);
            exit(1);
        }

        fillArrayWithNumbers(inputString, charactersReaded, arrayOfNumbers);

        // Обрезаем длину памяти
        if (ftruncate(sharedMemoryFD, sizeof(int) * numberOfNumbers) == -1) {
            perror("ftruncate (parent)");
            freeAllResources(semaphoreChild, semaphoreParent);
            free(arrayOfNumbers);
            free(inputString);
            exit(1);
        }

        // Получаем указатель на общую память
        if ((arrayInSharedMemory = mmap(NULL, sizeof(int) * numberOfNumbers, PROT_WRITE, MAP_SHARED,
                                        sharedMemoryFD, 0)) == MAP_FAILED) {
            perror("mmap (parent)");
            freeAllResources(semaphoreChild, semaphoreParent);
            free(arrayOfNumbers);
            free(inputString);
            exit(1);
        }

        memcpy(arrayInSharedMemory, arrayOfNumbers, sizeof(int) * numberOfNumbers);
        free(inputString);
        free(arrayOfNumbers);
        if (sem_post(semaphoreChild) == -1) {
            perror("sem_post (parent)");
            freeAllResources(semaphoreChild, semaphoreParent);
            exit(1);
        }
    }

    if (waitpid(pid, NULL, 0) == -1) {
        perror("WaitPID error (parent)");
        freeAllResources(semaphoreChild, semaphoreParent);
        exit(1);
    }

    freeAllResources(semaphoreChild, semaphoreParent);

    return 0;
}