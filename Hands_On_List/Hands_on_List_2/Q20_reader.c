/*
==================================
Author : Prakhar Sharma
Description : Q20.c -> 20. Write two programs so that both can communicate by FIFO - Use one way communication.
 
Date: 21st Sep, 2024.
==================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define FIFO_FILE "/tmp/my_fifo"

int main() {
    int fd;
    char buffer[100];

    fd = open(FIFO_FILE, O_RDONLY);
    read(fd, buffer, sizeof(buffer)); 

    printf("Reader: Received message: %s\n", buffer);

    close(fd); 

    return 0;
}

