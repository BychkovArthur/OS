#include "stdlib.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main() {
    int fd[2];
    pipe(fd);
    //                    будет считывание с тройки
    int array[8] = {1, 2, 3, 4, 5, 228, 7, 8};

    write(fd[1], array + 2, 6 * sizeof(int));
    int newArray[6];
    read(fd[0], newArray, 6 * sizeof(int));
    // read(fd[0], newArray + 2, 3 * sizeof(int));

    for (int i = 0; i < 5; i ++) {
        printf("New number is: %d\n", newArray[i]);
    }

}