#include <errno.h>
#include <linux/kernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define DATA_LENGTH 1024
#define MAX_ITERATE 14

#define __NR_init_buffer_421 442
#define __NR_enqueue_buffer_421 443
#define __NR_dequeue_buffer_421 444
#define __NR_delete_buffer_421 445

static sem_t mutex;

long init(void) {
    return syscall(__NR_init_buffer_421);
}

long enqueue(char* data) {
    return syscall(__NR_enqueue_buffer_421, data);
}

long dequeue(char* data) {
    return syscall(__NR_dequeue_buffer_421, data);
}

long delete(void) {
    return syscall(__NR_delete_buffer_421);
}

void* producer(void* arg) {
    char data[DATA_LENGTH];

    for (int i = 0; i < MAX_ITERATE; i++) {
        // usleep((rand() % 100) * 1000);
        // usleep(2 * 1000);
        usleep(80 * 1000);

        // sem_wait(&mutex);
        printf("Produced: %d\n", i % 10);

        memset(data, '0' + (i % 10), DATA_LENGTH);
        enqueue(data);
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
        usleep(2 * 1000);
        // sem_wait(&mutex);

        dequeue(data);

        printf("Consumed: %c\n", data[0]);

        printf("-------------------------------\n");
        // sem_post(&mutex);

        // printf("Consumed: %s\n", data);
    }
}

int main(int argc, char* argv[]) {
    pthread_t t1, t2;
    sem_init(&mutex, 0, 1);
    // sem_init(&pmutex, 0, 1);
    // sem_init(&cmutex, 0, 1);
    init();

    pthread_create(&t1, NULL, producer, NULL);
    pthread_create(&t2, NULL, consumer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    delete();
    sem_destroy(&mutex);
    // sem_destroy(&pmutex);
    // sem_destroy(&cmutex);
    return 0;
}