/*
==================================
Author : Prakhar Sharma
Description : Q10a. Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10 bytes (use lseek) and write again 10 bytes. a. check the return value of lseek
Date: 31st Aug, 2024.
==================================
*/

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 10

int main() {
    int fd = open("faltu.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        return 1;
    }

    char buf1[BUFFER_SIZE] = "pppppppppp";
    char buf2[BUFFER_SIZE] = "0000000000";

    if (write(fd, buf1, BUFFER_SIZE) != BUFFER_SIZE) {
        close(fd);
        return 1;
    }

    off_t offset = lseek(fd, 10, SEEK_CUR);
    if (offset == (off_t)-1) {
        close(fd);
        return 1;
    }

    if (write(fd, buf2, BUFFER_SIZE) != BUFFER_SIZE) {
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}

//Output:pppppppppp0000000000
