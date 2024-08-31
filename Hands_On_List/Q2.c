/*
==================================
Author : Prakhar Sharma
Description : Q2-> Write a simple program to execute in an infinite loop at the background. Go to /proc directory and identify all the process related information in the corresponding proc directory. 
Date: 31st Aug, 2024
==================================
*/

#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void list_proc() {
    DIR *proc_pntr;
    struct dirent *dir_pntr;

    proc_pntr = opendir("/proc");
    if (proc_pntr == NULL) {
        perror("Error opening /proc directory");
        exit(EXIT_FAILURE);
    }

    while ((dir_pntr = readdir(proc_pntr)) != NULL) {
        if (dir_pntr->d_type == DT_DIR) {
            // Check if the directory name is a valid PID (positive number)
            if (strtol(dir_pntr->d_name, NULL, 10) > 0) {
                printf("Process directory: %s\n", dir_pntr->d_name);
            }
        }
    }

    closedir(proc_pntr);
}

int main() {
    while (1) {
        list_proc();
        sleep(20);
    }

    return 0;
}

