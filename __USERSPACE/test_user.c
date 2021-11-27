#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>

#include "buffer.h"

#define MAX_ITERATE 100000

static sem_t mutex;
static int counter = 0;

void* producer(void* arg) {
    char data[DATA_LENGTH];

    for (int i = 0; i < MAX_ITERATE; i++) {
        usleep((rand() % 10) * 1000);

        memset(data, '0' + (i % 10), DATA_LENGTH);

        sem_wait(&mutex);
        printf(":: Enqueueing element into buffer. ::\n");
        printf("%c : %d\n", data[0], strlen(data));
        printf("%d items in buffer.\n", ++counter);
        sem_post(&mutex);

        enqueue_buffer_421(data);
    }
}

void* consumer(void* arg) {
    char data[DATA_LENGTH];
    
    for (int i = 0; i < MAX_ITERATE; i++) {
        usleep((rand() % 10) * 1000);
        
        dequeue_buffer_421(data);

        sem_wait(&mutex);
        printf(":: Dequeueing element into buffer. ::\n");
        printf("%c : %d\n", data[0], strlen(data));
        printf("%d items in buffer.\n", --counter);
        sem_post(&mutex);
    }
}

int main(int argc, char* argv[]) {
    srand((unsigned) time(NULL));

    pthread_t t1, t2;
    sem_init(&mutex, 0, 1);

    init_buffer_421();

    pthread_create(&t1, NULL, producer, NULL);
    pthread_create(&t2, NULL, consumer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    delete_buffer_421();
    sem_destroy(&mutex);

    return 0;
}