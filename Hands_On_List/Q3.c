/*
==================================
Author : Prakhar Sharma
Description : Q3-> Write a program to create a file and print the file descriptor value. Use creat() system call
Date: 31st Aug, 2024
==================================
*/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int arg_cnt, char *arg_vct[]) {
    int o = creat(arg_vct[1], S_IRWXU);
    printf("%d\n", o);
    return 0;
}
