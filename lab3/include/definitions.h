#ifndef DEFENITIONS_H
#define DEFENITIONS_H

#define NAME_MAX 255
#define ERROR 0
#define OK 1

#define SHARED_MEMORY_NAME "/shared_memory_lab3_OS"
#define SHARED_MEMORY_NAME_ERR "/shared_memory_err_lab3_OS"
#define SEMAPHORE_FOR_SHM_1 "/semaphore_shm_1_lab3_OS"
#define SEMAPHORE_FOR_SHM_2 "/semaphore_shm_2_lab3_OS"

// IPC
#define NOTHING 0
#define GENERAL_QUIT 1
#define CHILD_PROCESS_ERROR_QUIT 2
#define ERROR_SHM_SIZE (sizeof(unsigned char) * 1)
#define MAX_FD_LEN (4 + 1)

#endif  // DEFENITIONS_H