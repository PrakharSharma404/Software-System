#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    execl("/bin/ls", "ls", "-Rl", NULL);
    perror("execl failed");
    return 1;
}
