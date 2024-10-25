/*
==================================
Author : Prakhar Sharma
Description : Q8g.c -> 8. Write a separate program using signal system call to catch the following signals. 
 g. SIGPROF
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
    printf("Caught signal %d: Profiling timer expired (SIGPROF)\n", signum);
    exit(EXIT_SUCCESS);
}

int main() {
    struct itimerval timer;

    signal(SIGPROF, signal_catcher);

    timer.it_value.tv_sec = 3;
    timer.it_value.tv_usec = 0;

    setitimer(ITIMER_PROF, &timer, NULL);

    printf("Profiling timer set for 3 seconds of total CPU time (user + system)...\n");

    while (1) {
        for (double i = 0; i < INFINITY; ++i);
    }

    return 0;
}

