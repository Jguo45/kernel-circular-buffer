#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#include "buffer.h"

#define MAX_ITERATE 1000

sem_t mutex;
sem_t pmutex;
sem_t cmutex;

void* producer(void* arg) {
    char data[DATA_LENGTH];

    for (int i = 0; i < MAX_ITERATE; i++) {
        usleep((rand() % 100) * 1000);
        // usleep(2 * 1000);
        // usleep(40 * 1000);

        // sem_wait(&mutex);
        printf("Produced: %d\n", i % 10);

        memset(data, '0' + (i % 10), DATA_LENGTH);
        enqueue_buffer_421(data);
        printf("-------------------------------\n");
        // sem_post(&mutex);

        // printf("Produced: %s\n", data);
    }
}

void* consumer(void* arg) {
    char data[DATA_LENGTH];
    
    for (int i = 0; i < MAX_ITERATE; i++) {
        usleep((rand() % 100) * 1000);
        // usleep(80 * 1000);
        // usleep(2 * 1000);
        // sem_wait(&mutex);

        dequeue_buffer_421(data);

        printf("Consumed: %c\n", data[0]);

        printf("-------------------------------\n");
        // sem_post(&mutex);

        // printf("Consumed: %s\n", data);
    }
}

int main() {
    pthread_t t1, t2;
    sem_init(&mutex, 0, 1);
    sem_init(&pmutex, 0, 1);
    sem_init(&cmutex, 0, 1);
    init_buffer_421();

    pthread_create(&t1, NULL, producer, NULL);
    pthread_create(&t2, NULL, consumer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    delete_buffer_421();
    sem_destroy(&mutex);
    sem_destroy(&pmutex);
    sem_destroy(&cmutex);
    return 0;
}