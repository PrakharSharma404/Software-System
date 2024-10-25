/*
==================================
Author : Prakhar Sharma
Description : Q14.c -> 14. Write a simple program to create a pipe, write to the pipe, read from pipe and display on
the monitor.
Date: 21th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main() {
    char buff[22];
    int fd[2];

    pipe(fd);
    write(fd[1], "Hello from write end\n", 22);
    read(fd[0], buff, sizeof(buff));

    printf("Read from pipe: %s\n", buff);
    return 0;
}
