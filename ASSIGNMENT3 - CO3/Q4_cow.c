/* Copy-on-write demonstration with fork(): memory before and after the child writes. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec * 1e-9; }
static void mem(const char *tag)
{
    FILE *f = fopen("/proc/self/smaps_rollup", "r"); char l[256]; long rss = 0, pss = 0, shared = 0, priv = 0;
    if (!f) { f = fopen("/proc/self/statm", "r"); long a, b; if (f && fscanf(f, "%ld %ld", &a, &b) == 2) printf("%-28s resident pages: %ld\n", tag, b); if (f) fclose(f); return; }
    while (fgets(l, sizeof l, f)) {
        long v; if (sscanf(l, "Rss: %ld", &v) == 1) rss = v;
        else if (sscanf(l, "Pss: %ld", &v) == 1) pss = v;
        else if (sscanf(l, "Shared_Dirty: %ld", &v) == 1) shared += v;
        else if (sscanf(l, "Shared_Clean: %ld", &v) == 1) shared += v;
        else if (sscanf(l, "Private_Dirty: %ld", &v) == 1) priv += v;
    }
    fclose(f);
    printf("%-28s Rss=%7ld kB  Pss=%7ld kB  Shared=%7ld kB  Private_Dirty=%7ld kB\n", tag, rss, pss, shared, priv);
}

int main(int argc, char **argv)
{
    size_t mb = argc > 1 ? (size_t)atol(argv[1]) : 100, n = mb << 20;
    char *buf = malloc(n); memset(buf, 1, n);
    printf("parent allocated and wrote %zu MB\n", mb);
    mem("parent before fork:");
    fflush(stdout);
    int ready[2], go[2]; if (pipe(ready) || pipe(go)) return 1;
    double t0 = now(); pid_t pid = fork(); double tf = now() - t0;
    if (pid == 0) {
        struct rusage a, b; char c = 'x';
        mem("child right after fork:");
        getrusage(RUSAGE_SELF, &a);
        double t1 = now(); for (size_t i = 0; i < n; i += 4096) buf[i] = 2; double tw = now() - t1;
        getrusage(RUSAGE_SELF, &b);
        mem("child after writing all:");
        printf("child write loop: %.4f s, %ld page faults (each one is a copy-on-write page copy)\n", tw, b.ru_minflt - a.ru_minflt);
        if (write(ready[1], &c, 1) < 0) return 1;
        return 0;
    }
    char c; if (read(ready[0], &c, 1) < 0) return 1;
    waitpid(pid, NULL, 0);
    printf("fork() of a %zu MB process took %.3f ms (no page was copied at fork time)\n", mb, tf * 1000);
    return 0;
}
