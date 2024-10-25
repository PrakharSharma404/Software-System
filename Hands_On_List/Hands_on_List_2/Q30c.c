/*
==================================
Author : Prakhar Sharma
Description : 30. Write a program to create a shared memory.
              c. Detach the shared memory.

Date: 21st Sep, 2024.
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>

int main() {
    int shmid;
    key_t key = 1234; 
    char *shm_ptr;

    shmid = shmget(key, 1024, IPC_CREAT | 0666);
    shm_ptr = shmat(shmid, NULL, SHM_RDONLY);

    printf("Data in shared memory: %s\n", shm_ptr);

    shmdt(shm_ptr);

    return 0;
}
