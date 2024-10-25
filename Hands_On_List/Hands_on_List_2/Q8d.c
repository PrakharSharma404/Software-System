/*
==================================
Author : Prakhar Sharma
Description : Q8d.c -> 8. Write a separate program using signal system call to catch the following signals. 
 d. SIGALRM
Date: 21th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


void signal_catcher(int signum) {
    printf("Caught signal %d: Alarm (SIGALRM)\n", signum);
    exit(EXIT_SUCCESS); 
}

int main() {
    
    signal(SIGALRM, signal_catcher);

    printf("Setting alarm for 5 seconds...\n");
    alarm(5);

    
    while (1) {
        pause();  
    }

    return 0;
}
