/*
==================================
Author : Prakhar Sharma
Description : Q1.a-> Create a Soft link using system call
Date: 31st Aug, 2024.
==================================
*/

#include <unistd.h>

int main() {
    const char *trg_pth = "Q3.c";
    const char *lnk_pth = "system_call_sftlink";

    if (symlink(trg_pth, lnk_pth) == -1) {
        return 1;
    }

    printf("sucess");
    return 0;
}

// O/p: system_call_sftlink
