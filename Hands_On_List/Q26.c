/*
==================================
Author : Prakhar Sharma
Description : Q26-> Write a program to execute an executable program.
Date: 31st Aug, 2024
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    // Full path to the executable to be run
    const char *program_path = "D:/Hands on list/print_num.exe";

    // Execute the program
    execl(program_path, "print_num", (char *)NULL);

    // If execl returns, it means there was an error
    perror("Failed to execute the program");
    return 1;
}
