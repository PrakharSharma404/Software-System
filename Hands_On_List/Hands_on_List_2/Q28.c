/*
==================================
Author : Prakhar Sharma
Description : 28. Write a program to change the existing message queue permission. (use msqid_ds structure)

Date: 21st Sep, 2024.
==================================
*/

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

int main() {
    key_t key;
    int msgid;
    struct msqid_ds buf;

    key = ftok("progfile", 65);
    msgid = msgget(key, 0666 | IPC_CREAT);
    msgctl(msgid, IPC_STAT, &buf);

    buf.msg_perm.mode = 0644;
    msgctl(msgid, IPC_SET, &buf);

    printf("Message queue permissions changed to 0644\n");

    return 0;
}

