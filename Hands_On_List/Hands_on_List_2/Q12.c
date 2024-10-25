/*
==================================
Author : Prakhar Sharma
Description : Q12.c -> 12. Write a program to create an orphan process. Use kill system call to send SIGKILL signal to 
the parent process from the child process. 
Date: 21th Sep, 2024.
==================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t parent_pid = getpid();
    pid_t child_pid;

    if ((child_pid = fork()) < 0) {
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        sleep(1);
        printf("Child (PID %d) sending SIGKILL to Parent (PID %d)...\n", getpid(), parent_pid);

        if (kill(parent_pid, SIGKILL) < 0) {
            exit(EXIT_FAILURE);
        }

        sleep(1);
        printf("Child (PID %d) exiting...\n", getpid());
        exit(EXIT_SUCCESS);
    } else {
        printf("Parent (PID %d) running. Child PID: %d\n", parent_pid, child_pid);
        wait(NULL);
    }

    return 0;
}

