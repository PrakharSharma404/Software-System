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
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>

int main() {
    struct sockaddr_in serv, cli;
    int sd, sz, nsd;
    char buf[80];

    sd = socket(AF_UNIX, SOCK_STREAM, 0);

    serv.sin_family = AF_UNIX;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(5055);

    bind(sd, (void *)&serv, sizeof(serv));

    listen(sd, 5);
    sz = sizeof(cli);

    nsd = accept(sd, (void *)&cli, &sz);
    read(nsd, buf, sizeof(buf));

    printf("Message from Client: %s\n", buf);
    write(nsd, "ACK from Server\n", 17);

    return 0;
}
