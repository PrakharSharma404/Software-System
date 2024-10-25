/*
==================================
Author : Prakhar Sharma
Description : Q11.c -> 11. Write a program to ignore a SIGINT signal then reset the default action of the SIGINT signal - 
use sigaction system call. b. SIGINT signal - Use signal system call. 
Date: 21th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void ignore_sigint(int signum) {}

int main() {
    struct sigaction sa_ignore, sa_default;
    memset(&sa_ignore, 0, sizeof(sa_ignore));
    sa_ignore.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sa_ignore, NULL);

    printf("SIGINT signal is now ignored. Try pressing Ctrl + C...\n");
    sleep(10);

    memset(&sa_default, 0, sizeof(sa_default));
    sa_default.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sa_default, NULL);

    printf("SIGINT signal action reset to default. Press Ctrl + C again to terminate the program.\n");

    while (1) {
        pause();
    }

    return 0;
}
