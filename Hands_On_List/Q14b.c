/*
==================================
Author : Prakhar Sharma
Description : Q14-> Write a program to find the type of a file. a. Input should be taken from command line.
Date: 31st Aug, 2024
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

void print_file_type(mode_t mode) {
    if (S_ISREG(mode)) {
        printf("Regular file\n");
    } else if (S_ISDIR(mode)) {
        printf("Directory\n");
    } else if (S_ISLNK(mode)) {
        printf("Symbolic link\n");
    } else if (S_ISCHR(mode)) {
        printf("Character device\n");
    } else if (S_ISBLK(mode)) {
        printf("Block device\n");
    } else if (S_ISFIFO(mode)) {
        printf("FIFO\n");
    } else if (S_ISSOCK(mode)) {
        printf("Socket\n");
    } else {
        printf("Unknown\n");
    }
}

int main(int arg_cnt, char *arg_vct[]) {
    if (arg_cnt != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", arg_vct[0]);
        exit(EXIT_FAILURE);
    }

    const char *file_path = arg_vct[1];
    struct stat file_stat;

    if (stat(file_path, &file_stat) < 0) {
        fprintf(stderr, "File status not available\n");
        exit(EXIT_FAILURE);
    }

    print_file_type(file_stat.st_mode);

    return 0;
}

//Output: Regular file