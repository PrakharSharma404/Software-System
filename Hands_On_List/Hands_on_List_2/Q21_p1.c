/*
==================================
Author : Prakhar Sharma
Description : Q21.c -> 21. Write two programs so that both can communicate by FIFO - Use two way communications.
 
Date: 21st Sep, 2024.
==================================
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define FIFO1 "/tmp/fifo1"  // For Program 1 -> Program 2
#define FIFO2 "/tmp/fifo2"  // For Program 2 -> Program 1

int main() {
    char message1[] = "Hello from Program 1!";
    char buffer[100];

    // Create the FIFOs
    mkfifo(FIFO1, 0666);
    mkfifo(FIFO2, 0666);

    // Write to FIFO1 (Program 1 -> Program 2)
    int fd1 = open(FIFO1, O_WRONLY);
    write(fd1, message1, sizeof(message1));
    close(fd1);

    // Read from FIFO2 (Program 2 -> Program 1)
    int fd2 = open(FIFO2, O_RDONLY);
    read(fd2, buffer, sizeof(buffer));
    printf("Program 1 received: %s\n", buffer);
    close(fd2);

    return 0;
}
