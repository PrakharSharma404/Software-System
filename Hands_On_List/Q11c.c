/*
==================================
Author : Prakhar Sharma
Description : Q11.a-> Write a program to open a file, duplicate the file descriptor and append the file with both the descriptors and check whether the file is updated properly or not.
c. use fcntl 
Date: 31st Aug, 2024
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    const char *file_path = "faltu_file.txt";

    // Open file with read/write, create if not exist, and append mode
    int pehli_file = open(file_path, O_RDWR | O_CREAT | O_APPEND, 0644);
    if (pehli_file < 0) {
        perror("Failed to open file");
        return 1;
    }

    // Duplicate file descriptor using fcntl
    int doosri_file = fcntl(pehli_file, F_DUPFD, 3);
    if (doosri_file < 0) {
        perror("Failed to duplicate file descriptor");
        close(pehli_file);
        return 1;
    }

    // Write to the file using the first file descriptor
    if (write(pehli_file, "Written pehli_file\n", 20) < 0) {
        perror("Failed to write with pehli_file descriptor");
        close(pehli_file);
        close(doosri_file);
        return 1;
    }

    // Write to the file using the duplicated file descriptor
    if (write(doosri_file, "Written doosri_file\n", 21) < 0) {
        perror("Failed to write with doosri_file descriptor");
        close(pehli_file);
        close(doosri_file);
        return 1;
    }

    // Close both file descriptors
    close(pehli_file);
    close(doosri_file);

    // Open the file to check contents (this part is more for validation)
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Failed to open file for reading");
        return 1;
    }

    fclose(file);

    return 0;
}
