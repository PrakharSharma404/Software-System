/*
==================================
Author : Prakhar Sharma
Description : Q11.a-> Write a program to open a file, duplicate the file descriptor and append the file with both the descriptors and check whether the file is updated properly or not.
b. use dup2
Date: 31st Aug, 2024
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    const char *file_path = "faltu_file.txt";

    int pehli_file = open(file_path, O_RDWR | O_CREAT | O_APPEND, 0644);
    if (pehli_file < 0) {
        return 1;
    }

    int doosri_file = dup2(pehli_file, 3);
    if (doosri_file < 0) {
        close(pehli_file);
        return 1;
    }

    if (write(pehli_file, "Written pehli_file\n", 32) < 0) {
        close(pehli_file);
        close(doosri_file);
        return 1;
    }

    if (write(doosri_file, "Written doosri_file\n", 33) < 0) {
        close(pehli_file);
        close(doosri_file);
        return 1;
    }

    close(pehli_file);
    close(doosri_file);

    FILE *file = fopen(file_path, "r");
    if (!file) {
        return 1;
    }

    fclose(file);

    return 0;
}
