/*
==================================
Author : Prakhar Sharma
Description : Q8c.c -> 8. Write a separate program using signal system call to catch the following signals. 
 c. SIGFPE 
Date: 21th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>


void signal_catcher(int signum) {
    printf("Signal %d: Exception (SIGFPE)\n", signum);
    exit(EXIT_FAILURE); 
}

int main() {
    
    signal(SIGFPE, signal_catcher);

    int numerator = 5;
    int denominator = 0;
    int result;

    printf("Division.....\n");

    
    result = numerator / denominator;

    
    printf("Result: %d\n", result);

    return 0;
}
