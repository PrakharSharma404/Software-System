/*
==================================
Author : Prakhar Sharma
Description : Q8f.c -> 8. Write a separate program using signal system call to catch the following signals. 
 f. SIGVTALRM
Date: 21th Sep, 2024.
==================================
*/
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

void signal_catcher(int signum) {
    printf("Caught signal %d: Virtual timer expired (SIGVTALRM)\n", signum);
    exit(EXIT_SUCCESS);
}

int main() {
    struct itimerval timer;

    signal(SIGVTALRM, signal_catcher);

    timer.it_value.tv_sec = 2;
    timer.it_value.tv_usec = 0;

    setitimer(ITIMER_VIRTUAL, &timer, NULL);

    printf("Virtual timer set for 2 seconds of CPU time...\n");

    while (1) {
        for (double i = 0; i < INFINITY; ++i);
    }

    return 0;
}
