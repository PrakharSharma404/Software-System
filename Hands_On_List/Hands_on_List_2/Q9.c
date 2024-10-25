/*
==================================
Author : Prakhar Sharma
Description : Q9.c -> 9. Write a program to ignore a SIGINT signal then reset the default action of the SIGINT 
signal - Use signal system call. Date: 21th Sep, 2024.
==================================
*/
#include <stdio.h> 
#include <signal.h>
#include <unistd.h>

int main() {
    signal(SIGINT, SIG_IGN);
    printf("SIGINT signal is now ignored. Try pressing Ctrl + C...\n");

    sleep(10);

    signal(SIGINT, SIG_DFL);
    printf("SIGINT signal action reset to default. Press Ctrl + C again to terminate the program.\n");

    while (1) {
        pause();
    }

    return 0;
}



