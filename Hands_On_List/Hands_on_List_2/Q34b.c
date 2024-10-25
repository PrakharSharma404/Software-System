/*
==================================
Author : Prakhar Sharma
Description : 34. Write a program to create a concurrent server.
b. Use pthread_create.

Date: 21st Sep, 2024.
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void *handle_client(void *arg) {
    int client_fd = *(int *)arg;
    char buffer[BUFFER_SIZE];
    
    int valread = read(client_fd, buffer, BUFFER_SIZE);
    buffer[valread] = '\0';
    printf("Received: %s\n", buffer);

    const char *response = "Message received\n";
    send(client_fd, response, strlen(response), 0);

    close(client_fd);
    return NULL;
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *)&client_fd);
        pthread_detach(thread_id); 
    }

    return 0;
}
