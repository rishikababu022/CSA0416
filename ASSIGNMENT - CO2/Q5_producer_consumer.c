#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 5
#define ITEMS 20

static int buffer[SIZE], in = 0, out = 0;
static sem_t empty_slots, full_slots;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static long consumed_sum = 0;

static void *producer(void *arg) {
    (void)arg;
    for (int item = 1; item <= ITEMS; ++item) {
        sem_wait(&empty_slots);
        pthread_mutex_lock(&lock);
        buffer[in] = item;
        in = (in + 1) % SIZE;
        pthread_mutex_unlock(&lock);
        sem_post(&full_slots);
    }
    return NULL;
}

static void *consumer(void *arg) {
    (void)arg;
    for (int k = 0; k < ITEMS; ++k) {
        sem_wait(&full_slots);
        pthread_mutex_lock(&lock);
        int item = buffer[out];
        out = (out + 1) % SIZE;
        pthread_mutex_unlock(&lock);
        sem_post(&empty_slots);

        pthread_mutex_lock(&lock);
        consumed_sum += item;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(void) {
    pthread_t p, c;
    if (sem_init(&empty_slots, 0, SIZE) != 0 ||
        sem_init(&full_slots, 0, 0) != 0) {
        perror("sem_init");
        return EXIT_FAILURE;
    }

    if (pthread_create(&p, NULL, producer, NULL) != 0 ||
        pthread_create(&c, NULL, consumer, NULL) != 0) {
        fprintf(stderr, "Could not create threads\n");
        return EXIT_FAILURE;
    }
    pthread_join(p, NULL);
    pthread_join(c, NULL);

    printf("items consumed: %d, sum = %ld (expected %d)\n",
           ITEMS, consumed_sum, ITEMS * (ITEMS + 1) / 2);

    sem_destroy(&empty_slots);
    sem_destroy(&full_slots);
    pthread_mutex_destroy(&lock);
    return EXIT_SUCCESS;
}
