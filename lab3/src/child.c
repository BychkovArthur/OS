#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../include/definitions.h"
#include "../include/functions.h"

int main(int argc, char* argv[]) {
    struct stat sd;
    FILE* outputFile;
    sem_t* semaphoreParent;
    sem_t* semaphoreChild;
    int childProcessExitStatus = 0;
    int* arrayInSharedMemory;
    unsigned char* errorsAndExitStatus;

    // mmap для ошибок
    if ((errorsAndExitStatus = mmap(NULL, ERROR_SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, STDOUT_FILENO,
                                    0)) == MAP_FAILED) {
        perror("mmap_err (child)");
        exit(1);
    }

    if ((semaphoreParent = sem_open(SEMAPHORE_FOR_SHM_1, O_CREAT, S_IRWXU, 1)) == SEM_FAILED ||
        (semaphoreChild = sem_open(SEMAPHORE_FOR_SHM_2, O_CREAT, S_IRWXU, 0)) == SEM_FAILED) {
        perror("semaphore (child)");
        errorsAndExitStatus[0] = CHILD_PROCESS_ERROR_QUIT;
        exit(1);
    }

    if (argc != 2) {
        fprintf(stderr, "Invalid argumetns\n");
        errorsAndExitStatus[0] = CHILD_PROCESS_ERROR_QUIT;
        if (sem_post(semaphoreParent) == -1) {
            perror("sem_post (child)");
        }

        exit(1);
    }

    if ((outputFile = fopen(argv[1], "w")) == NULL) {
        fprintf(stderr, "Can't open file (child)\n");
        errorsAndExitStatus[0] = CHILD_PROCESS_ERROR_QUIT;
        if (sem_post(semaphoreParent) == -1) {
            perror("sem_post (child)");
        }
        exit(1);
    }

    while (1) {
        if (sem_wait(semaphoreChild) == -1) {
            perror("sem_wait (child)");
            exit(1);
        }

        if (errorsAndExitStatus[0] == GENERAL_QUIT) {
            if (sem_post(semaphoreParent) == -1) {
                perror("sem_post (child)");
                exit(1);
            }
            break;
        }

        if (fstat(STDIN_FILENO, &sd) == -1) {
            perror("Can't get file size (child)");
            errorsAndExitStatus[0] = CHILD_PROCESS_ERROR_QUIT;
            fclose(outputFile);
            if (sem_post(semaphoreParent) == -1) {
                perror("sem_post (child)");
            }
            exit(1);
        }

        if ((arrayInSharedMemory = mmap(NULL, sd.st_size, PROT_READ, MAP_SHARED, STDIN_FILENO, 0)) ==
            MAP_FAILED) {
            perror("mmap (child)");
            errorsAndExitStatus[0] = CHILD_PROCESS_ERROR_QUIT;
            fclose(outputFile);
            if (sem_post(semaphoreParent) == -1) {
                perror("sem_post (child)");
            }
            exit(1);
        }

        double resultOfDivision =
            devide(arrayInSharedMemory, sd.st_size / sizeof(int), &childProcessExitStatus);
        if (resultOfDivision == -1.0 && childProcessExitStatus) {
            errorsAndExitStatus[0] = CHILD_PROCESS_ERROR_QUIT;
            fclose(outputFile);
            if (sem_post(semaphoreParent) == -1) {
                perror("sem_post (child)");
            }
            exit(1);
        }

        if (fprintf(outputFile, "%le\n", resultOfDivision) < 0) {
            perror("Can't write in output file (child)");
            errorsAndExitStatus[0] = CHILD_PROCESS_ERROR_QUIT;
            fclose(outputFile);
            if (sem_post(semaphoreParent) == -1) {
                perror("sem_post (child)");
            }
            exit(1);
        }

        if (sem_post(semaphoreParent) == -1) {
            perror("sem_post (child)");
        }
    }

    if (sem_close(semaphoreParent) == -1 || sem_close(semaphoreChild) == -1) {
        perror("semaphore close (child)");
        errorsAndExitStatus[0] = CHILD_PROCESS_ERROR_QUIT;
        fclose(outputFile);
        if (sem_post(semaphoreParent) == -1) {
            perror("sem_post (child)");
        }
        exit(1);
    }

    fclose(outputFile);
    return 0;
}