/*
==================================
Author : Prakhar Sharma
Description : Q13.c -> 13. Write two programs: first program is waiting to catch SIGSTOP signal, the second program 
will send the signal (using kill system call). Find out whether the first program is able to catch 
the signal or not.   
Date: 21th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) exit(EXIT_FAILURE);

    pid_t receiver_pid = atoi(argv[1]);

    printf("Sending SIGSTOP to PID %d\n", receiver_pid);
    kill(receiver_pid, SIGSTOP);

    sleep(2);

    printf("Sending SIGCONT to PID %d\n", receiver_pid);
    kill(receiver_pid, SIGCONT);

    return 0;
}
