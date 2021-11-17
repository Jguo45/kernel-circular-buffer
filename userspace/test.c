#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include "buffer.h"

#define MAX_ITERATE 50

// void* producer(void* arg) {
//     for (int i = 0; i < MAX_ITERATE; i++) {
//         char* value;
//         char* data;
//         sprintf(value, "%d", i % 10);
        
//         usleep((rand() % 100) * 1000);
//         memset(data, i % 10, DATA_LENGTH);
//         enqueue_buffer_421(data);
//         printf("Produced: %s\n", data);
//     }
// }

// void* consumer(void* arg) {
//     for (int i = 0; i < MAX_ITERATE; i++) {
//         char *data;
        
//         usleep((rand() % 100) * 1000);
//         dequeue_buffer_421(data);
//         printf("Consumed: %s\n", data);
//     }
// }

void* test1(void* arg) {
    for (int i = 0; i < 10; i++) {
        // int timeout = rand() % 5;
        // sleep(timeout);

        printf("Thread 1: Value - %d Timeout - %d\n", i, 0);
    }
}

void* test2(void* arg) {
    for (int i = 0; i < 10; i++) {
        // int timeout = rand() % 5;
        // sleep(timeout);

        printf("Thread 2: Value - %d Timeout - %d\n", i, 0);
    }
}

int main() {
    // pthread_t t1, t2;
    // // init_buffer_421();
    // pthread_create(&t1, NULL, test1, NULL);
    // pthread_create(&t2, NULL, test2, NULL);

    // pthread_join(t1, NULL);
    // pthread_join(t2, NULL);
    char data[DATA_LENGTH];

    init_buffer_421();

    for (int i = 0; i < 20; i++) {
        memset(data, '0' + (i % 10), DATA_LENGTH);
        enqueue_buffer_421(data);
        // printf("%s\n", data);
    }
    char *dequeue;
    for (int i = 0; i < 20; i++) {
        dequeue_buffer_421(dequeue);
        printf("%s\n", dequeue);
    }

    delete_buffer_421();
    return 0;
}