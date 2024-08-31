/*
==================================
Author : Prakhar Sharma
Description : Q7-> Write a program to copy file1 into file2 ($cp file1 file2).
Date: 31st Aug, 2024
==================================
*/

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        return 1;
    }

    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1) {
        return 1;
    }

    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        close(src_fd);
        return 1;
    }

    char buf[BUFFER_SIZE];
    ssize_t bytes_read, bytes_written;

    while ((bytes_read = read(src_fd, buf, sizeof(buf))) > 0) {
        bytes_written = write(dest_fd, buf, bytes_read);
        if (bytes_written != bytes_read) {
            close(src_fd);
            close(dest_fd);
            return 1;
        }
    }

    if (bytes_read == -1) {
        close(src_fd);
        close(dest_fd);
        return 1;
    }

    close(src_fd);
    close(dest_fd);

    return 0;
}

