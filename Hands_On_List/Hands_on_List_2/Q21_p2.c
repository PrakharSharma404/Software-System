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

#define FIFO1 "/tmp/fifo1" 
#define FIFO2 "/tmp/fifo2"  

int main() {
    char message2[] = "Hello from Program 2!";
    char buffer[100];

    
    int fd1 = open(FIFO1, O_RDONLY);
    read(fd1, buffer, sizeof(buffer));
    printf("Program 2 received: %s\n", buffer);
    close(fd1);

    
    int fd2 = open(FIFO2, O_WRONLY);
    write(fd2, message2, sizeof(message2));
    close(fd2);

    return 0;
}

