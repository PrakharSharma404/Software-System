#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/resource.h>

struct timespec clock_get() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts;
}

unsigned long long time_diff_ns(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000000000ULL + (end.tv_nsec - start.tv_nsec);
}

int main() {
    int i;
    unsigned long long t1, t2, total_ns = 0;
    struct timespec start, end;

    for (i = 0; i < 100; i++) {
        start = clock_get();
        getppid();
        end = clock_get();
        total_ns += time_diff_ns(start, end);  // Measure the time difference in nanoseconds
    }

    printf("Total time to execute 100 getppid(): %llu nanoseconds\n", total_ns);
    return 0;
}
