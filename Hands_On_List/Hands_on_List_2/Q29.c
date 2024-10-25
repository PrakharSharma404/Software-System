/*
==================================
Author : Prakhar Sharma
Description : 29. Write a program to remove the message queue.

Date: 21st Sep, 2024.
==================================
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main() {
    key_t key;
    int msgid;

    key = ftok("progfile", 65);
    msgid = msgget(key, 0666);

    if (msgctl(msgid, IPC_RMID, NULL) == 0) {
        printf("Message queue removed successfully.\n");
    } else {
        printf("Failed to remove message queue.\n");
    }

    return 0;
}

