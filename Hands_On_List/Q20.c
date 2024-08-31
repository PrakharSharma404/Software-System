/*
==================================
Author : Prakhar Sharma
Description : Q20-> Find out the priority of your running program. Modify the priority with nice command
Date: 31st Aug, 2024
==================================
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int arg_cnt, char *arg_vct[]) {
    int cur_pri, new_pri, change;

    if (arg_cnt != 2) {
        printf("Usage: %s <priority adjustment>\n", arg_vct[0]);
        return 1;
    }

    change = atoi(arg_vct[1]);

    errno = 0;
    cur_pri = nice(0);

    if (cur_pri == -1 && errno != 0) {
        printf("Failed\n");
        return 1;
    }

    printf("Current priority: %d\n", cur_pri);

    errno = 0;
    new_pri = nice(change);

    if (new_pri == -1 && errno != 0) {
        printf("Failed\n");
        return 1;
    }

    printf("New priority: %d\n", new_pri);

    return 0;
}

