#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>

void print_rlimit(int resource, const char *name) {
    struct rlimit rl;
    if (getrlimit(resource, &rl) == 0) {
        printf("%s:\n", name);
        printf("  Soft limit: %ld\n", rl.rlim_cur);
        printf("  Hard limit: %ld\n", rl.rlim_max);
    } else {
        perror("getrlimit");
    }
}

int main() {
    struct rlimit rl;

    
    rl.rlim_cur = 1024; 
    rl.rlim_max = 2048;  
    if (setrlimit(RLIMIT_NOFILE, &rl) != 0) {
        perror("setrlimit");
        return 1;
    }

    
    print_rlimit(RLIMIT_CPU, "CPU Time Limit");
    print_rlimit(RLIMIT_FSIZE, "File Size Limit");
    print_rlimit(RLIMIT_DATA, "Data Area Limit");
    print_rlimit(RLIMIT_STACK, "Stack Size Limit");
    print_rlimit(RLIMIT_CORE, "Core Dump Size Limit");
    print_rlimit(RLIMIT_NOFILE, "Number of File Descriptors Limit");
    print_rlimit(RLIMIT_AS, "Address Space Limit");
    print_rlimit(RLIMIT_NPROC, "Number of Processes Limit");

    while (1) {
        pause(); 
    }

    return 0;
}
