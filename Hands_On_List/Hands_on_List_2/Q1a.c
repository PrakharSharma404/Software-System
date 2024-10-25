/*
==================================
Author : Prakhar Sharma
Description : Q1a.c -> 1. Write a separate program (for each time domain) to set an interval timer in 10 sec and 10 microseconds.
a. ITIMER_REAL
Date: 20th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

void timer_handler(int signum) {
    printf("10sec timer over\n");
}

int main() {
    struct itimerval mytimer;

    signal(SIGALRM, timer_handler);

    mytimer.it_value.tv_sec = 10;
    mytimer.it_value.tv_usec = 10;

    mytimer.it_interval.tv_sec = 10;
    mytimer.it_interval.tv_usec = 10;

    setitimer(ITIMER_REAL, &mytimer, NULL);

    while (1) {
        pause();
    }

    return 0;
}

