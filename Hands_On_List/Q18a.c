/*
==================================
Author : Prakhar Sharma
Description : Q18.b->Write a program to perform Record locking.
 a. Implement write lock

Date: 31st Aug, 2024
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

// Function to lock the file
void lock_file(int fd) {
    struct flock lock;
    lock.l_type = F_WRLCK;    // Set the lock type to write lock
    lock.l_whence = SEEK_SET; // Lock the entire file
    lock.l_start = 0;
    lock.l_len = 0;          // 0 means lock until EOF

    // Attempt to acquire the lock
    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror("Error locking file");
        _exit(1);
    }
}

// Function to unlock the file
void unlock_file(int fd) {
    struct flock lock;
    lock.l_type = F_UNLCK;    // Set the lock type to unlock
    lock.l_whence = SEEK_SET; // Unlock the entire file
    lock.l_start = 0;
    lock.l_len = 0;          // 0 means unlock until EOF

    // Attempt to release the lock
    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror("Error unlocking file");
        _exit(1);
    }
}

int main(int arg_cnt, char *arg_vct[]) {
    int fd;
    struct {
        int trn_no;
        int tick_cnt;
    } db[3];

    int i = 0;
    while (i < 3) {
        db[i].trn_no = i + 1000;
        db[i].tick_cnt = i;
        i++;
    }

    fd = open("Faltu_file.txt", O_CREAT | O_RDWR | O_APPEND, 0766);    // Open a file
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Lock the file before writing
    lock_file(fd);

    write(fd, &db, sizeof(db));

    // Unlock the file after writing
    unlock_file(fd);

    close(fd);
    return 0;
}

