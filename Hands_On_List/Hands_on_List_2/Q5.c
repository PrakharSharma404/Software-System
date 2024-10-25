/*
==================================
Author : Prakhar Sharma
Description : Q5.c -> 5. Write a program to print the system limitation of 
 a. maximum length of the arguments to the exec family of functions.
 b. maximum number of simultaneous processes per user id.
 c. number of clock ticks (jiffy) per second.
 d. maximum number of open files.
 e. size of a page.
 f. total number of pages in the physical memory.
 g. number of currently available pages in the physical memory.
Date: 21th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <sys/sysinfo.h>

void print_limits() {
    long arg_max;
    long max_processes;
    long clk_tck;
    long max_open_files;
    long page_size;
    struct sysinfo sys_info;

    arg_max = sysconf(_SC_ARG_MAX);
    printf("Maximum length of arguments to exec functions: %ld bytes\n", arg_max);

    max_processes = sysconf(_SC_NPROCESSORS_CONF);
    printf("Maximum number of simultaneous processes per user ID: %ld\n", max_processes);

    clk_tck = sysconf(_SC_CLK_TCK);
    printf("Number of clock ticks (jiffies) per second: %ld\n", clk_tck);

    max_open_files = sysconf(_SC_OPEN_MAX);
    printf("Maximum number of open files: %ld\n", max_open_files);

    page_size = sysconf(_SC_PAGE_SIZE);
    printf("Page size: %ld bytes\n", page_size);

    sysinfo(&sys_info);
    printf("Total number of pages in physical memory: %ld\n", sys_info.totalram / (page_size / 1024));

    printf("Number of currently available pages in physical memory: %ld\n", sys_info.freeram / (page_size / 1024));
}

int main() {
    print_limits();
    return 0;
}
