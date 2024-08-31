/*
==================================
Author : Prakhar Sharma
Description : Q9.c-> A program to print the following information about a given file: uid
Date: 31st Aug, 2024.
==================================
*/

#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    struct stat st;

    if (stat(argv[1], &st) == -1) return 1;

    printf("UID : %u\n", st.st_uid);

    return 0;
}

//Output: UID : 1653

