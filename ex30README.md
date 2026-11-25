#include <stdio.h>
#include <pthread.h>

void *fun(void *arg)
{
    printf("Thread is running\n");

    pthread_exit(NULL);
}

int main()
{
    pthread_t t1;

    pthread_create(&t1, NULL, fun, NULL);

    if(pthread_equal(t1, pthread_self()))
        printf("Threads are equal\n");
    else
        printf("Threads are not equal\n");

    pthread_join(t1, NULL);

    printf("Thread joined successfully\n");

    return 0;
}<img width="351" height="148" alt="Screenshot 2026-11-25 230922" src="https://github.com/user-attachments/assets/8bc70452-e951-423a-81d8-36d3e58b9e9b" />
