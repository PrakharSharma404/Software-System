/*
==================================
Author : Prakhar Sharma
Description : 26. Write a program to send messages to the message queue. Check $ipcs -q

Date: 21st Sep, 2024.
==================================
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

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
    
    message.msg_type = 1;
    strcpy(message.msg_text, "Hello, this is a test message!");
    msgsnd(msgid, &message, sizeof(message), 0);

    printf("Message sent: %s\n", message.msg_text);

    return 0;
}

