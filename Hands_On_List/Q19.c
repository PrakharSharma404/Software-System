/*
==================================
Author : Prakhar Sharma
Description : Q19-> Write a program to find out time taken to execute getpid system call. Use time stamp counter
Date: 31st Aug, 2024
==================================
*/

#include <stdio.h>
#include <unistd.h>

unsigned long long rdtsc() {
    unsigned int min, max;
    asm volatile("rdtsc" : "=a"(min), "=d"(max));
    return ((unsigned long long)max << 32) | min;
}

int main() {
    unsigned long long start, finish;
    pid_t pid;

    start = rdtsc();
    pid = getpid();
    finish = rdtsc();

    printf("Time taken: %llu cycles\n", finish - start);
    printf("ID: %d\n", pid);

    return 0;
}
