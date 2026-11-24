/* Baseline for the comparison: 1,000 commands with fork()+execlp(). */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec/1e9;}
int main(int argc, char **argv)
{
    int n = argc > 1 ? atoi(argv[1]) : 1000;
    double t0 = now();
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) { execlp("true", "true", (char *)NULL); _exit(127); }
        int st; waitpid(pid, &st, 0);
    }
    printf("fork+exec : %d commands in %.4f s (%.1f us each)\n", n, now()-t0, (now()-t0)/n*1e6);
    return 0;
}
