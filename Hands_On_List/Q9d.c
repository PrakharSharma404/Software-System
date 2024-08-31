/*
==================================
Author : Prakhar Sharma
Description : Q9.d-> A program to print the following information about a given file: gid
Date: 31st Aug, 2024.
==================================
*/

#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    struct stat st;

    if (stat(argv[1], &st) == -1) return 1;

    printf("GID : %u\n", st.st_gid);

    return 0;
}

//Output: GID : 1017