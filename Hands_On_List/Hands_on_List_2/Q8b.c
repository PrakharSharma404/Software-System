/*
==================================
Author : Prakhar Sharma
Description : Q8b.c -> 8. Write a separate program using signal system call to catch the following signals.
Date: 21th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void signal_catcher(int signum) {
    printf("Caught signal %d: Interrupt (SIGINT)\n", signum);
    exit(EXIT_SUCCESS);  
}

int main() {
    signal(SIGINT, signal_catcher);

    while (1) {
        printf("Press Ctrl+C\n");
        sleep(1); 
    }

    return 0;
}

