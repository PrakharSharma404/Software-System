/*
==================================
Author : Prakhar Sharma
Description : 27. Write a program to receive messages from the message queue.
              a. with 0 as a flag

Date: 21st Sep, 2024.
==================================
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

int main() {
    key_t key;
    int msgid;
    struct msg_buffer message;

    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);
    msgrcv(msgid, &message, sizeof(message), 0, 0);

    printf("Received message: %s\n", message.msg_text);

    return 0;
}
