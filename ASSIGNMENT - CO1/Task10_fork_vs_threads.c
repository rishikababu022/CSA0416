/* Task 10: time to create 1,000 processes (fork) vs 1,000 threads (pthread_create). */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/wait.h>
#define N 1000
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec/1e9;}
static void *worker(void *a){ (void)a; return NULL; }
int main(void)
{
    /* touch 50 MB so fork() has a realistic address space to copy page tables for */
    size_t sz = 50u << 20; char *big = malloc(sz); for (size_t i = 0; i < sz; i += 4096) big[i] = 1;

    double t0 = now();
    for (int i = 0; i < N; i++) { pid_t p = fork(); if (p == 0) _exit(0); waitpid(p, NULL, 0); }
    double tp = now() - t0;

    t0 = now();
    for (int i = 0; i < N; i++) { pthread_t t; pthread_create(&t, NULL, worker, NULL); pthread_join(t, NULL); }
    double tt = now() - t0;

    printf("%d processes (fork+wait)       : %.4f s  (%.1f us each)\n", N, tp, tp/N*1e6);
    printf("%d threads (create+join)       : %.4f s  (%.1f us each)\n", N, tt, tt/N*1e6);
    printf("processes / threads time ratio : %.1fx\n", tp/tt);
    free(big);
    return 0;
}
