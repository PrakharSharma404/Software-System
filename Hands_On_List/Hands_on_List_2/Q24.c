/*
==================================
Author : Prakhar Sharma
Description : 24. Write a program to create a message queue and print the key and message queue id.

Date: 21st Sep, 2024.
==================================
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

int main() {
    key_t key;
    int msgid;

    key = ftok("progfile", 65); 

    msgid = msgget(key, 0666 | IPC_CREAT);

    printf("Message Queue Key: %d\n", key);
    printf("Message Queue ID: %d\n", msgid);

    return 0;
}
