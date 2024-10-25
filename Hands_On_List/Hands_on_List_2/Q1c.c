/*
==================================
Author : Prakhar Sharma
Description : Q1c.c -> 1. Write a separate program to set an interval timer in 10 sec and 10 microseconds.
c. ITIMER_PROF
Date: 20th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void timer_handler(int signum) {
    printf("10 sec timer over (ITIMER_PROF)\n");
}

int main() {
    struct itimerval mytimer;

    signal(SIGPROF, timer_handler);

    mytimer.it_value.tv_sec = 10;
    mytimer.it_value.tv_usec = 10;

    mytimer.it_interval.tv_sec = 10;
    mytimer.it_interval.tv_usec = 10;

    setitimer(ITIMER_PROF, &mytimer, NULL);

    while (1) {
        pause();
    }

    return 0;
}
