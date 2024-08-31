/*
==================================
Author : Prakhar Sharma
Description : Q29-> Write a program to get scheduling policy and modify the scheduling policy (SCHED_FIFO, SCHED_RR).
Date: 31st Aug, 2024
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/types.h>
#include <errno.h>

void print_scheduler(int policy) {
    switch (policy) {
        case SCHED_FIFO:
            printf("SCHED_FIFO\n");
            break;
        case SCHED_RR:
            printf("SCHED_RR\n");
            break;
        case SCHED_OTHER:
            printf("SCHED_OTHER\n");
            break;
        default:
            printf("Unknown\n");
    }
}

int main() {
    int policy;
    struct sched_param param;

    policy = sched_getscheduler(0);
    if (policy == -1) {
        exit(EXIT_FAILURE);
    }

    print_scheduler(policy);

    param.sched_priority = 1;
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        exit(EXIT_FAILURE);
    }

    printf("Policy is SCHED_FIFO now\n");

    policy = sched_getscheduler(0);
    if (policy == -1) {
        exit(EXIT_FAILURE);
    }
    print_scheduler(policy);

    param.sched_priority = 1;
    if (sched_setscheduler(0, SCHED_RR, &param) == -1) {
        exit(EXIT_FAILURE);
    }

    printf("Policy is SCHED_RR now\n");

    policy = sched_getscheduler(0);
    if (policy == -1) {
        exit(EXIT_FAILURE);
    }
    print_scheduler(policy);

    return 0;
}
