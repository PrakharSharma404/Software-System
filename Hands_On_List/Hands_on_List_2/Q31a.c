/*
==================================
Author : Prakhar Sharma
Description : 31. Write a program to create a semaphore and initialize value to the semaphore.
              a. Create a binary semaphore.

Date: 21st Sep, 2024.
==================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>

int main (void) {
    union semun arg;
    int key, semid;

    key = ftok(".", 'a');
    semid = semget(key, 1, IPC_CREAT | 0644);
    arg.val = 1;
    semctl(semid, 0, SETVAL, arg);
    
    return 0;
}
