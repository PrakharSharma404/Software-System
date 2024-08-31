#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    execvp("ls", (char *[]){"ls", "-Rl", NULL});
    perror("execvp failed");
    return 1;
}
