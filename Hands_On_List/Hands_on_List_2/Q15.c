#
/*
==================================
Author : Prakhar Sharma
Description : Q15.c -> Write a simple program to send some data from parent to the child process.
Date: 21th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    char buff[19];

    pipe(fd);

    if (fork() == 0) {
        close(fd[1]); 
        printf("CHILD\n");
        read(fd[0], buff, sizeof(buff));
        printf("Message got from parent:\n");
        printf("%s", buff);
    } else {
        close(fd[0]); 
        printf("PARENT\n");
        printf("Messaging the child\n");
        write(fd[1], "Hello from parent\n", 19);
        wait(0);
    }

    return 0;
}
