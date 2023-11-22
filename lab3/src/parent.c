#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../include/definitions.h"
#include "../include/functions.h"

// TODO:
// - Надо сделать, чтобы в независимости от запуска через бинарник или через make все работало одинаково

int main() {
    char fileName[NAME_MAX + 1];
    if (readFileName(fileName) == ERROR) {
        printf("Invalid file name %s (parent)\n", fileName);
        return 1;
    }

    int sharedMemoryFD = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, S_IRWXU);

    if (sharedMemoryFD == -1) {
        perror("shm_open (parent)");
        return 1;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork (parent)");
        return 1;
    }

    if (pid == 0) { // Child process
        if (dup2(sharedMemoryFD, STDIN_FILENO) == -1) {
            perror("dup2 stdin (child)");
            return 1;
        }

        if (execl("./build/ch   ild_exe", "./build/child_exe", fileName, NULL) == -1) {
            perror("exec error (child)");
            return 1;
        }
    }
}