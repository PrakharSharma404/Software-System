/*
==================================
Author : Prakhar Sharma
Description : Q1.c-> Create a FIFO file (mkfifo Library Function or mknod system call).
Date: 31st Aug, 2024.
==================================
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    const char *fifo = "fifo_file";

    if (mkfifo(fifo, 0666) == -1) {
        return 1;
    }

    return 0;
}

