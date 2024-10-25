/*
==================================
Author : Prakhar Sharma
Description : Q10.c -> 10. Write a separate program using sigaction system call to catch the following signals. 
b. SIGINT
signal - Use signal system call. Date: 21th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void sigint_handler(int signum, siginfo_t *info, void *context) {
    printf("Caught signal %d (SIGINT)\n", signum);
    exit(EXIT_SUCCESS);
}

int main() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = sigint_handler;
    sa.sa_flags = SA_SIGINFO;

    sigaction(SIGINT, &sa, NULL);

    printf("Waiting for Ctrl + C (SIGINT)...\n");

    while (1) {
        sleep(1);
    }

    return 0;
}

