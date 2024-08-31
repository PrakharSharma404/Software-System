/*
==================================
Author : Prakhar Sharma
Description : Q5-> Write a program to create five new files with infinite loop. Execute the program in the background and check the file descriptor table at /proc/pid/fd.
Date: 31st Aug, 2024
==================================
*/

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int arg_cnt, char *arg_vct[]) {
    int f1 = creat(arg_vct[1], S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    int f2 = creat(arg_vct[2], S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    int f3 = creat(arg_vct[3], S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    int f4 = creat(arg_vct[4], S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
    int f5 = creat(arg_vct[5], S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

    printf("f1=%d\tf2=%d\tf3=%d\tf4=%d\tf5=%d\t\n", f1, f2, f3, f4, f5);

    while (1);
}


