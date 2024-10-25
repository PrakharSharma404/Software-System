/*
==================================
Author : Prakhar Sharma
Description : Q8e.c -> 8. Write a separate program using signal system call to catch the following signals. 
 e. SIGALRM
Date: 21th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>


void signal_catcher(int signum) {
    printf("Caught signal %d: Alarm (SIGALRM)\n", signum);
    exit(EXIT_SUCCESS);
}

int main() {
    struct itimerval timer;

    
    signal(SIGALRM, signal_catcher);

    
    timer.it_value.tv_sec = 5;
    timer.it_value.tv_usec = 0;

    
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1) {
        perror("setitimer");
        return 1;
    }

    printf("Timer set for 5 seconds...\n");

    
    while (1) {
        pause();
    }

    return 0;
}
