/*
==================================
Author : Prakhar Sharma
Description : Q13-> Write a program to wait for a STDIN for 10 seconds using select. Write a proper print statement to verify whether the data is available within 10 seconds or not (check in $man 2 select)
Date: 31st Aug, 2024
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>

int main() {
    int fd;
    fd_set read_fds;
    struct timeval time_ten;
    int result;

    fd = open("faltu_file.txt", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open");
        exit(EXIT_FAILURE);
    }

    FD_ZERO(&read_fds);
    FD_SET(fd, &read_fds);

    time_ten.tv_sec = 10;
    time_ten.tv_usec = 0;

    result = select(fd + 1, &read_fds, NULL, NULL, &time_ten);

    if (result < 0) {
        perror("Failed");
        close(fd);
        exit(EXIT_FAILURE);
    } else if (result == 0) {
        fprintf(stderr, "No data available\n");
        close(fd);
        exit(EXIT_FAILURE);
    } else if (FD_ISSET(fd, &read_fds)) {
        printf("Data available\n");
    } else {
        fprintf(stderr, "Error\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    close(fd);
    return 0;
}

//Output: Data available