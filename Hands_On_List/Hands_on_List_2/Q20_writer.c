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
#include <sys/stat.h>

#define FIFO_FILE "/tmp/my_fifo"

int main() {
    int fd;
    char message[] = "Hello from writer!";

    mkfifo(FIFO_FILE, 0666);

    fd = open(FIFO_FILE, O_WRONLY); 
    write(fd, message, sizeof(message)); 

    close(fd); 

    return 0;
}

