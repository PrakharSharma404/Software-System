/*
==================================
Author : Prakhar Sharma
Description : Q7.c -> 7. Write a simple program to print the created thread ids.
Date: 21th Sep, 2024.
==================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


void *thread_function(void *arg) {
    pthread_t thread_id = pthread_self();
    printf("Thread ID: %lu\n", (unsigned long)thread_id);
    return NULL;
}

int main() {
    pthread_t threads[3];
    int result_code;


    for (int i = 0; i < 3; i++) {
        result_code = pthread_create(&threads[i], NULL, thread_function, NULL);
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
