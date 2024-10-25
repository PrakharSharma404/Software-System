/*
==================================
Author : Prakhar Sharma
Description : Q13.c -> 13. Write two programs: first program is waiting to catch SIGSTOP signal, the second program 
will send the signal (using kill system call). Find out whether the first program is able to catch 
the signal or not. This is Receiver 
Date: 21th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int s) {
    if (s == SIGCONT) {
        printf("Caught SIGCONT, resumed process\n");
    }
    if (s == SIGSTOP) {
        printf("Caught SIGSTOP, process stopped\n");
    }
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = 0;

    sigaction(SIGSTOP, &sa, NULL);
    sigaction(SIGCONT, &sa, NULL);

    printf("Receiver PID: %d\n", getpid());
    printf("Waiting for SIGSTOP (which can't be caught)\n");

    while (1) {
        pause();
    }

    return 0;
}
