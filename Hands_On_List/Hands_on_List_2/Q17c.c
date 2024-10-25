/*
==================================
Author : Prakhar Sharma
Description : 17. Write a program to execute ls -l | wc.
c. use fcntl
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int pipefd[2];
    pid_t pid;

    pipe(pipefd);
    pid = fork();

    if (pid == 0) {
        close(pipefd[1]);
        fcntl(pipefd[0], F_DUPFD, STDIN_FILENO);
        close(pipefd[0]);
        execlp("wc", "wc", NULL);
    } else {
        close(pipefd[0]);
        fcntl(pipefd[1], F_DUPFD, STDOUT_FILENO);
        close(pipefd[1]);
        execlp("ls", "ls", "-l", NULL);
    }

    return 0;
}
