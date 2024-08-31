/*
==================================
Author : Prakhar Sharma
Description : Q12-> Write a program to find out the opening mode of a file. Use fcntl.
Date: 31st Aug, 2024
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    const char *file_path = "faltu_file";

    int fd = open(file_path, O_RDWR | O_CREAT | O_APPEND, 0644);
    if (fd < 0) {
        return 1;
    }

    int flags = fcntl(fd, F_GETFL);
    if (flags < 0) {
        close(fd);
        return 1;
    }

    printf("Opening Mode:\n");
    if (flags & O_RDONLY) {
        printf("  Read-only\n");
    }
    if (flags & O_WRONLY) {
        printf("  Write-only\n");
    }
    if (flags & O_RDWR) {
        printf("  Read/Write\n");
    }
    if (flags & O_APPEND) {
        printf("  Append mode\n");
    }

    close(fd);

    return 0;
}

/*Output: Opening Mode:
  Read/Write */
  

