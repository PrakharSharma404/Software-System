/*
==================================
Author : Prakhar Sharma
Description : Q28-> Write a program to get maximum and minimum real time priority.
Date: 31st Aug, 2024
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <limits.h>

int main() {
    int max_pri, min_pri;

    max_pri = sched_get_priority_max(SCHED_FIFO);
    if (max_pri == -1) {
        exit(EXIT_FAILURE);
    }

    min_pri = sched_get_priority_min(SCHED_FIFO);
    if (min_pri == -1) {
        exit(EXIT_FAILURE);
    }

    printf("Maximum real-time priority for SCHED_FIFO: %d\n", max_pri);
    printf("Minimum real-time priority for SCHED_FIFO: %d\n", min_pri);

    return 0;
}
