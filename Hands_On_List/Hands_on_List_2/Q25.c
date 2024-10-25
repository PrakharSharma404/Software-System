// 25. Write a program to print a message queue's (use msqid_ds and ipc_perm structures)
// a. access permission
// b. uid, gid
// c. time of last message sent and received
// d. time of last change in the message queue
// d. size of the queue
// f. number of messages in the queue
// g. maximum number of bytes allowed
// h. pid of the msgsnd and msgrcv

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

int main() {
    int msgid;
    key_t key;
    struct msqid_ds buf;
    struct ipc_perm perms;

    key = ftok(".", 'b');
    msgid = msgget(key, IPC_CREAT | 0666);
    msgctl(msgid, IPC_STAT, &buf);

    perms = buf.msg_perm;

    printf("UID: %u\n", perms.uid);
    printf("GID: %u\n", perms.gid);
    printf("Last Sent: %s", ctime(&buf.msg_stime));
    printf("Last Received: %s", ctime(&buf.msg_rtime));
    printf("Last Change: %s", ctime(&buf.msg_ctime));
    printf("Size (bytes): %lu\n", buf.__msg_cbytes);
    printf("Messages: %lu\n", buf.msg_qnum);
    printf("Max Bytes: %lu\n", buf.msg_qbytes);
    printf("Last msgsnd PID: %d\n", buf.msg_lspid);
    printf("Last msgrcv PID: %d\n", buf.msg_lrpid);

    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
