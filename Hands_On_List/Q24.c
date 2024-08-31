/*
==================================
Author : Prakhar Sharma
Description : Q24-> Write a program to create an orphan process.
Date: 31st Aug, 2024
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid > 0) {
        printf("Old Parent PID: %d\n", getpid());
        printf("Child PID: %d\n", pid);
        sleep(5);
    } else if (pid == 0) {
        printf("Child Process Initial Parent PID: %d\n", getppid());
        sleep(10);
        printf("Child Process New Parent PID: %d\n", getppid());
    } else {
        exit(EXIT_FAILURE);
    }

    return 0;
}


