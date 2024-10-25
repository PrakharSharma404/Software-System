/*
==================================
Author : Prakhar Sharma
Description : 33. Write a program to communicate between two machines using socket.

Date: 21st Sep, 2024.
==================================
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    struct sockaddr_in serv;
    int sd;
    char buf[80];

    sd = socket(AF_UNIX, SOCK_STREAM, 0);

    serv.sin_family = AF_UNIX;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(5055);

    connect(sd, (void *)&serv, sizeof(serv));
    write(sd, "hello server\n", 14);

    read(sd, buf, sizeof(buf));

    printf("Message from server: %s\n", buf);

    return 0;
}
