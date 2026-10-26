#include <stdio.h>
#include <pthread.h>

void *task(void *arg) {
    printf("Thread is running\n");
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, task, NULL);
    pthread_create(&t2, NULL, task, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Main program completed\n");

    return 0;
}<img width="320" height="145" alt="Screenshot 2026-10-26 224139" src="https://github.com/user-attachments/assets/748c0c33-18a9-48ac-9ae9-4301d7342187" />
