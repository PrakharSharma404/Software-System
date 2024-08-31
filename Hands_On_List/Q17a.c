/*
==================================
Author : Prakhar Sharma
Description : Q17.a->  17. Write a program to simulate online ticket reservation. Implement write lock, Write a program to open a file, store a ticket number and exit. Write a separate program, to open the file, implement write lock, read the ticket number, increment the number and print the new ticket number then close the file.
Date: 31st Aug, 2024
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    struct {
        int tick_no;
    } db;

    struct flock lk;
    int Descrp;
    
    Descrp = open("database", O_RDWR);
    if (Descrp == -1) {
        perror("Failed to open file");
        return 1;
    }

    lk.lk_type = F_WRLCK;
    lk.lk_whence = SEEK_SET;
    lk.lk_start = 0;
    lk.lk_len = 0;
    lk.lk_pid = getpid();

    fcntl(Descrp, F_SETLKW, &lk);

    read(Descrp, &db, sizeof(db));
    printf("Ticket number: %d\n", db.tick_no);
    
    db.tick_no++;
    
    lseek(Descrp, 0, SEEK_SET);
    write(Descrp, &db, sizeof(db));    
    
    lk.lk_type = F_UNLCK;
    fcntl(Descrp, F_SETLK, &lk);

    close(Descrp);

    return 0;
}
