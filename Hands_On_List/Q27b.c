#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    execlp("ls", "ls", "-Rl", NULL);
    perror("execlp failed");
    return 1;
}
