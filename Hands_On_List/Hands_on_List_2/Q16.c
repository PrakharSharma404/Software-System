/*
==================================
Author : Prakhar Sharma
Description : Q16.c -> Write a program to send and receive data from parent to child vice versa. Use two-way communication.
Date: 21th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int fd1[2];
    int fd2[2];
    char buff1[50], buff2[50];

    pipe(fd1);
    pipe(fd2);

    if (fork() == 0) {
        close(fd1[1]);
        read(fd1[0], buff2, sizeof(buff2));
        printf("Message From Parent: %s\n", buff2);
        close(fd2[0]);
        printf("Enter Message for Parent: ");
        scanf(" %[^\n]", buff1);
        write(fd2[1], buff1, sizeof(buff1));
    } else {
        close(fd1[0]); 
        printf("Enter Message for Child: ");
        scanf("%[^\n]", buff2);
        write(fd1[1], buff2, sizeof(buff2));
        close(fd2[1]);
        read(fd2[0], buff1, sizeof(buff1));
        printf("Message from Child: %s\n", buff1);
        wait(0);
    }

    return 0;
}
