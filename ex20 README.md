#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t mutex, wrt;
int readers = 0, data = 0;

void *reader(void *arg) {
    sem_wait(&mutex);
    readers++;
    if (readers == 1)
        sem_wait(&wrt);
    sem_post(&mutex);

    printf("Reader reads: %d\n", data);

    sem_wait(&mutex);
    readers--;
    if (readers == 0)
        sem_post(&wrt);
    sem_post(&mutex);

    return NULL;
}

void *writer(void *arg) {
    sem_wait(&wrt);
    data++;
    printf("Writer writes: %d\n", data);
    sem_post(&wrt);
    return NULL;
}

int main() {
    pthread_t r, w;

    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    pthread_create(&r, NULL, reader, NULL);
    pthread_create(&w, NULL, writer, NULL);

    pthread_join(r, NULL);
    pthread_join(w, NULL);

    sem_destroy(&mutex);
    sem_destroy(&wrt);
    return 0;
}<img width="310" height="101" alt="Screenshot 2026-11-03 135135" src="https://github.com/user-attachments/assets/00ce3dd1-dc1e-4bcc-a462-05d7bc675d5c" />
