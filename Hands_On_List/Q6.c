/*
==================================
Author : Prakhar Sharma
Description : Q6-> Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls.
Date: 31st Aug, 2024
==================================
*/

#include <unistd.h>

int main() {
    char buf[4096];
    ssize_t br;

    while ((br = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        write(STDOUT_FILENO, buf, br);
    }

    return 0;
}

