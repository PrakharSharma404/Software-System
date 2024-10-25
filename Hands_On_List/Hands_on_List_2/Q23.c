/*
==================================
Author : Prakhar Sharma
Description : Write a program to print the maximum number of files that can be opened within a process
and the size of a pipe's circular buffer.

Date: 21st Sep, 2024.
==================================
*/

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>

int main() {
    
    long max_open_files = sysconf(_SC_OPEN_MAX);  
    printf("Maximum number of files that can be opened within a process: %ld\n", max_open_files);

    int pipefd[2];
    pipe(pipefd); 
    int pipe_size = fcntl(pipefd[1], F_GETPIPE_SZ); 
    printf("Size of the pipe's circular buffer: %d bytes\n", pipe_size);

    return 0;
}

