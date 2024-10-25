/*
==================================
Author : Prakhar Sharma
Description : Q6.c -> 6. Write a simple program to create three threads.
Date: 21th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void *thread_function(void *arg) {
    int thread_num = *((int *)arg);
    printf("Thread %d!\n", thread_num);
    return NULL;
}

int main() {
    pthread_t threads[3];
    int thread_args[3];
    int result_code;

    
    for (int i = 0; i < 3; i++) {
        thread_args[i] = i + 1;  
        result_code = pthread_create(&threads[i], NULL, thread_function, &thread_args[i]);
        if (result_code != 0) {
            fprintf(stderr, "Error creating thread %d: %d\n", i + 1, result_code);
            exit(EXIT_FAILURE);
        }
    }

    
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

