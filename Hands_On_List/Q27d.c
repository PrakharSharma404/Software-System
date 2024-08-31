#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    char *args[] = {"ls", "-Rl", NULL};
    execv("/bin/ls", args);
    perror("execv failed");
    return 1;
}
