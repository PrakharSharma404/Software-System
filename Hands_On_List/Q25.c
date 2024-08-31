/*
==================================
Author : Prakhar Sharma
Description : Q25-> Write a program to create three child processes. The parent should wait for a particular child (use waitpid system call).
Date: 31st Aug, 2024
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t c1, c2, c3;
    pid_t child_pid;
    int status;

    c1 = fork();
    if (c1 < 0) {
        exit(EXIT_FAILURE);
    } else if (c1 == 0) {
        printf("c1: PID = %d, Parent PID = %d\n", getpid(), getppid());
        sleep(2);
        exit(1);
    }

    c2 = fork();
    if (c2 < 0) {
        exit(EXIT_FAILURE);
    } else if (c2 == 0) {
        printf("c2: PID = %d, Parent PID = %d\n", getpid(), getppid());
        sleep(4);
        exit(2);
    }

    c3 = fork();
    if (c3 < 0) {
        exit(EXIT_FAILURE);
    } else if (c3 == 0) {
        printf("c3: PID = %d, Parent PID = %d\n", getpid(), getppid());
        sleep(6);
        exit(3);
    }

    printf("Parent PID = %d\n", getpid());

    child_pid = waitpid(c2, &status, 0);

    if (child_pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (WIFEXITED(status)) {
        printf("c2 with PID %d exited with status %d\n", child_pid, WEXITSTATUS(status));
    } else {
        printf("c2 with PID %d did not exit normally\n", child_pid);
    }

    waitpid(c1, NULL, 0);
    waitpid(c3, NULL, 0);

    return 0;
}
