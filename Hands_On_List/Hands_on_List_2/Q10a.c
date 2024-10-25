/*
==================================
Author : Prakhar Sharma
Description : Q10.c -> 10. Write a separate program using sigaction system call to catch the following signals. 
 a. SIGSEGV 
signal - Use signal system call. Date: 21th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

void sigsegv_handler(int signum, siginfo_t *info, void *context) {
    printf("Caught signal %d (SIGSEGV)\n", signum);
    exit(EXIT_FAILURE);
}

int main() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = sigsegv_handler;
    sa.sa_flags = SA_SIGINFO;

    sigaction(SIGSEGV, &sa, NULL);

    printf("Causing a segmentation fault...\n");

    int *ptr = NULL;
    *ptr = 58;

    return 0;
}

