/*
==================================
Author : Prakhar Sharma
Description : Write a program to wait for data to be written into FIFO within 10 seconds.
Uses the select system call with FIFO to check for data availability.

Date: 21st Sep, 2024.
==================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_NAME "/tmp/myfifo"

int main() {
    int fifo_fd;
    char buffer[100];
    fd_set readfds;
    struct timeval timeout;

    mkfifo(FIFO_NAME, 0666); 

    fifo_fd = open(FIFO_NAME, O_RDONLY | O_NONBLOCK);

    FD_ZERO(&readfds);
    FD_SET(fifo_fd, &readfds);

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    select(fifo_fd + 1, &readfds, NULL, NULL, &timeout); 

    if (FD_ISSET(fifo_fd, &readfds)) {
        read(fifo_fd, buffer, sizeof(buffer));
        printf("Data read from FIFO: %s\n", buffer); 
    } else {
        printf("No data written to the FIFO within 10 seconds.\n"); 
    }

    close(fifo_fd);
    unlink(FIFO_NAME);  

    return 0;
}
