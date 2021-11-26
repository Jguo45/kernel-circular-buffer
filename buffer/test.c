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
#define MAX_ITERATE 24

#define __NR_init_buffer_421 442
#define __NR_enqueue_buffer_421 443
#define __NR_dequeue_buffer_421 444
#define __NR_delete_buffer_421 445

static sem_t mutex;         // mutex used for printing
static int counter = 0;

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
        // usleep((rand() % 10) * 1000);
        usleep(2 * 1000);
        // usleep(80 * 1000);

        memset(data, '0' + (i % 10), DATA_LENGTH);

        sem_wait(&mutex);
        printf(":: Enqueueing element into buffer. ::\n");
        printf("%c : %d\n", data[0], strlen(data));
        printf("%d items in buffer.\n\n", ++counter);
        sem_post(&mutex);

        enqueue(data);
    }
}

void* consumer(void* arg) {
    char data[DATA_LENGTH];
    
    for (int i = 0; i < MAX_ITERATE; i++) {
        // usleep((rand() % 10) * 1000);
        usleep(80 * 1000);
        // usleep(2 * 1000);
        
        dequeue(data);

        sem_wait(&mutex);
        printf(":: Dequeueing element into buffer. ::\n");
        printf("%c : %d\n", data[0], strlen(data));
        printf("%d items in buffer.\n\n", --counter);
        sem_post(&mutex);
    }
}

int main(int argc, char* argv[]) {
    srand((unsigned) time(NULL));

    pthread_t t1, t2;           // initialize threads
    sem_init(&mutex, 0, 1);     // initialize mutexs

    init();                     // initialize buffer

    pthread_create(&t1, NULL, producer, NULL);
    pthread_create(&t2, NULL, consumer, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    delete();					// delete buffer
    sem_destroy(&mutex);

    return 0;
}