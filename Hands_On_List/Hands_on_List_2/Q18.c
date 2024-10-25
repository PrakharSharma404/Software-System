/*
==================================
Author : Prakhar Sharma
Description : Q18.c -> 18. Write a program to find out total number of directories on the pwd. 
              Execute ls -l | grep ^d | wc using only dup2.
 
Date: 21th Sep, 2024.
==================================
*/

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
    int fd1[2];
    int fd2[2];

    pipe(fd1);
    pipe(fd2);

    if(fork() == 0){
        close(fd1[0]);
        close(fd2[0]);
        close(fd2[1]);
        dup2(fd1[1], 1);
        execlp("ls", "ls", "-l", (char *)NULL);
    }

    else{
        if(fork() == 0){
            close(fd1[1]);
            close(fd2[0]);
            dup2(fd1[0], 0);
            dup2(fd2[1], 1);
            execlp("grep", "grep", "^d", (char *)NULL);
        }

        else{
            close(fd2[1]);
            close(fd1[0]);
            close(fd1[1]);
            dup2(fd2[0], 0);
            execlp("wc", "wc", (char *)NULL);
        }
    }
}
