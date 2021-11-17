#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#include "buffer.h"

#define MAX_ITERATE 100

// sem_t pmutex;
// sem_t cmutex;

void* producer(void* arg) {
    char data[DATA_LENGTH];

    for (int i = 0; i < MAX_ITERATE; i++) {
        usleep((rand() % 100) * 1000);
        // usleep(2 * 1000);
        // usleep(40 * 1000);

        // printf("test\n");

        printf("Produced: %d\n", i % 10);
        memset(data, '0' + (i % 10), DATA_LENGTH);
        enqueue_buffer_421(data);

        // printf("Produced: %s\n", data);
    }
}

void* consumer(void* arg) {
    char data[DATA_LENGTH];
    for (int i = 0; i < MAX_ITERATE; i++) {
        usleep((rand() % 100) * 1000);
        // usleep(40 * 1000);
        // usleep(2 * 1000);

        dequeue_buffer_421(data);

        printf("Consumed: %c\n", data[0]);

        // printf("Consumed: %s\n", data);
    }
}

int main() {
    pthread_t t1, t2;
    init_buffer_421();

    pthread_create(&t1, NULL, producer, NULL);
    pthread_create(&t2, NULL, consumer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    delete_buffer_421();
    return 0;
}