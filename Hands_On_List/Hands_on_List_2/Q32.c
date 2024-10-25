/*
==================================
Author : Prakhar Sharma
Description : 32. Write a program to implement semaphore to protect any critical section.
              

Date: 21st Sep, 2024.
==================================
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>

int main(void) {
    int key, semid;
    key = ftok(".", 'a');
    struct sembuf buf = {0, -1, 0}; 
    semid = semget(key, 1, 0);

    printf("Waiting for unlock...\n");
    printf("pid = %d\n", getpid());
    semop(semid, &buf, 1);
    printf("In critical section...\n");

    printf("Press ENTER to unlock...\n");
    getchar();

    buf.sem_op = 1;
    semop(semid, &buf, 1);

    return 0;
}
