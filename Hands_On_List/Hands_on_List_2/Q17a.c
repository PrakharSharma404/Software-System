/*
==================================
Author : Prakhar Sharma
Description : 17. Write a program to execute `ls -l | wc`. 
a. Use `dup` for redirection.
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int pipefd[2];
    pid_t pid;

    pipe(pipefd);
    pid = fork();

    if (pid == 0) {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        execlp("wc", "wc", NULL);
    } else {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execlp("ls", "ls", "-l", NULL);
    }

    return 0;
}
