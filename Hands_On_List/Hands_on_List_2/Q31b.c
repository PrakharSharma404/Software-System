/*
==================================
Author : Prakhar Sharma
Description : 31. Write a program to create a semaphore and initialize value to the semaphore.
              b. Create a counting semaphore.

Date: 21st Sep, 2024.
==================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
};

int main(void) {
    union semun arg;
    int key, semid;

    key = ftok(".", 'a');
    semid = semget(key, 1, IPC_CREAT | 0644);
    arg.val = 5;
    
    if (semctl(semid, 0, SETVAL, arg) == -1) {
        return 1;
    }

    printf("Semaphore initialized with a counting value of %d\n", arg.val);

    return 0;
}

