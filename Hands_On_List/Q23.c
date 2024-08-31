/*
==================================
Author : Prakhar Sharma
Description : Q23-> Write a program to create a Zombie state of the running program.
Date: 31st Aug, 2024
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    printf("%d\n", getpid());
    int pid = fork();
    printf("%d\n", getpid());

    if (pid < 0) {
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("%d\n", pid); // Child process: exit immediately to become a zombie
        exit(0);
    } else {
        // Parent process: keep printing 2 continuously
        while (1) {
            printf("2\n");
            sleep(1); // Sleep to slow down the output
        }
    }

    return 0;
}
