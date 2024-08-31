/*
==================================
Author : Prakhar Sharma
Description : Q16.b-> Write a program to perform mandatory locking.
b. Implement read lock
Date: 31st Aug, 2024
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    const char *file_path = "rd_lck.txt";
    int fd;
    struct flock lock;

    fd = open(file_path, O_RDWR | O_CREAT, 0644);
    if (fd < 0) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    lock.l_type = F_RDLCK;   // Read lock
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;          // Lock the entire file

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

    printf("Read lock applied successfully to %s. You can now read the file.\n", file_path);

    // Sleep for 10 seconds to demonstrate the lock
    sleep(10);

    // Close the file descriptor
    close(fd);
    return 0;
}
