/*
==================================
Author : Prakhar Sharma
Description : Q4-> Write a program to open an existing file with read write mode. Try O_EXCL flag also.  
Date: 31st Aug, 2024
==================================
*/

#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("descrptr_file", O_RDWR | O_EXCL);
    if (fd == -1) {
        return 1;
    }

    close(fd);
    return 0;
}

