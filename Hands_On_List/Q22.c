/*
==================================
Author : Prakhar Sharma
Description : Q22-> Write a program, open a file, call fork, and then write to the file by both the child as well as the parent processes. Check output of the file.
Date: 31st Aug, 2024
==================================
*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main(int arg_cnt, char *arg_vct[]) {
    if (arg_cnt != 2) {
        printf("Usage: %s <filename>\n", arg_vct[0]);
        return 1;
    }

    char *filename = arg_vct[1];
    pid_t child_pid;
    int fd;

    fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, S_IRWXU);

    if (fd < 0) {
        return 1;
    }

    child_pid = fork();

    if (child_pid < 0) {
        close(fd);
        return 1;
    }

    if (child_pid != 0) {
        write(fd, "Parent ran first\n", 18);
    } else {
        write(fd, "Child ran first\n", 16);
    }

    close(fd);
    return 0;
}
