/* Alternative concept: run 1,000 commands with posix_spawn() instead of fork()+exec(). */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <time.h>
#include <sys/wait.h>
extern char **environ;
static double now(void){struct timespec t;clock_gettime(CLOCK_MONOTONIC,&t);return t.tv_sec+t.tv_nsec/1e9;}
int main(int argc, char **argv)
{
    int n = argc > 1 ? atoi(argv[1]) : 1000;
    char *args[] = {"true", NULL};
    double t0 = now();
    for (int i = 0; i < n; i++) {
        pid_t pid; int st;
        if (posix_spawnp(&pid, "true", NULL, NULL, args, environ) != 0) { perror("spawn"); return 1; }
        waitpid(pid, &st, 0);
    }
    printf("posix_spawn: %d commands in %.4f s (%.1f us each)\n", n, now()-t0, (now()-t0)/n*1e6);
    return 0;
}
