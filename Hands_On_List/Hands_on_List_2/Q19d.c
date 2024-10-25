/*
==================================
Author : Prakhar Sharma
Description : Q19.c -> 19. Create a FIFO file using the mknod system call.
 
Date: 21st Sep, 2024.
==================================
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    const char *fifo_file = "my_fifo";

    mknod(fifo_file, S_IFIFO | 0666, 0); 

    return 0;
}
