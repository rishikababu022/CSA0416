#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;
int count = 0;

void *increment(void *arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&lock);
        count++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_mutex_init(&lock, NULL);

    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Final count = %d\n", count);

    pthread_mutex_destroy(&lock);
    return 0;
}<img width="302" height="151" alt="Screenshot 2026-11-03 134926" src="https://github.com/user-attachments/assets/34af0547-2170-4160-8a62-ff02629b5bfb" />
