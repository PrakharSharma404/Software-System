/*
==================================
Author : Prakhar Sharma
Description : Q16.a-> Write a program to perform mandatory locking.
a. Implement write lock
Date: 31st Aug, 2024
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    const char *file_path = "faltu_file.txt";
    int fd;
    struct flock lock;

    // Open the file in read/write mode and create it if it doesn't exist
    fd = open(file_path, O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Set up the file lock structure
    lock.l_type = F_WRLCK;  // Write lock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;  // Lock the entire file

    // Apply the lock
    if (fcntl(fd, F_SETLK, &lock) < 0) {
        // Check if the error is due to the lock being unavailable
        if (errno == EACCES || errno == EAGAIN) {
            perror("Lock unavailable");
        } else {
            perror("Failed to apply lock");
        }
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Write lock applied successfully to %s.\n", file_path);

    // Sleep for 10 seconds to demonstrate the lock
    sleep(10);

    // Close the file descriptor
    close(fd);
    return 0;
}
