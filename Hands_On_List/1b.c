/*
==================================
Author : Prakhar Sharma
Description : Q1.b-> Create a Hard link using system call
Date: 31st Aug, 2024.
==================================
*/

#include <unistd.h>

int main() {
    const char *trg_pth = "Q3.c";
    const char *lnk_pth = "new_syscl_Hrdlnk";

    if (link(trg_pth, lnk_pth) == -1) {
        return 1;
    }

    return 0;
}

//Output: new_syscl_Hrdlnk
