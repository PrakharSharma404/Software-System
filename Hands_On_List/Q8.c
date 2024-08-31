/*
==================================
Author : Prakhar Sharma
Description : Q8-> Write a program to open a file in read only mode, read line by line and display each line as it is read. Close the file when end of file is reached.
Date: 31st Aug, 2024
==================================
*/

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    char buf[BUFFER_SIZE];
    ssize_t bytes_read;
    size_t i = 0;

    while ((bytes_read = read(fd, buf + i, sizeof(buf) - i)) > 0) {
        for (size_t j = 0; j < bytes_read; j++) {
            if (buf[i + j] == '\n') {
                buf[i + j] = '\0';
                write(STDOUT_FILENO, buf, i + j + 1);
                i = 0;
            } else {
                i++;
            }
        }
    }

    if (bytes_read == -1) {
        perror("read");
    }

    if (i > 0) {
        buf[i] = '\0';
        write(STDOUT_FILENO, buf, i);
    }

    close(fd);
    return 0;
}

/*Output: this
is
a
text
file*/
