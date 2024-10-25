/*
==================================
Author : Prakhar Sharma
Description : Q19.c -> 19. Create a FIFO file using the mkfifo library function.
 
Date: 21st Sep, 2024.
==================================
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    const char *fifo_file = "my_fifo";

    
    mkfifo(fifo_file, 0666);

    return 0;
}

