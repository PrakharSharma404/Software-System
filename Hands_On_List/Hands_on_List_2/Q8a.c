/*
==================================
Author : Prakhar Sharma
Description : Q8a.c -> 8. Write a separate program using signal system call to catch the following signals.
 a. SIGSEGV 
Date: 21th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>


void signal_catcher(int signum) {
    printf("Caught signal %d: Segmentation Fault (SIGSEGV)\n", signum);
    exit(EXIT_FAILURE);  
}

int main() {
    
    signal(SIGSEGV, signal_catcher);

    
    int *ptr = NULL;
    *ptr = 58;

    return 0;
}



