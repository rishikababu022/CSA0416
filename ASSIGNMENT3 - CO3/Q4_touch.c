/* Touch memory in sequential and random page order and count page faults + time.
 * Usage: ./Q4_touch SIZE_MB [passes]
 * To go beyond free RAM use a VM or a memory limit, e.g.
 *   systemd-run --user --scope -p MemoryMax=512M -p MemorySwapMax=2G ./Q4_touch 1024 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/resource.h>

static double now(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec * 1e-9; }
static void faults(long *mn, long *mj) { struct rusage r; getrusage(RUSAGE_SELF, &r); *mn = r.ru_minflt; *mj = r.ru_majflt; }

int main(int argc, char **argv)
{
    size_t mb = argc > 1 ? (size_t)atol(argv[1]) : 64; int passes = argc > 2 ? atoi(argv[2]) : 2;
    long ps = sysconf(_SC_PAGESIZE); size_t n = mb * 1048576 / ps;
    char *a = mmap(NULL, n * ps, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (a == MAP_FAILED) { perror("mmap"); return 1; }
    size_t *order = malloc(n * sizeof *order);
    for (size_t i = 0; i < n; i++) order[i] = i;
    srand(1); for (size_t i = n - 1; i > 0; i--) { size_t j = (size_t)rand() % (i + 1); size_t t = order[i]; order[i] = order[j]; order[j] = t; }

    printf("size %zu MB = %zu pages of %ld B\n", mb, n, ps);
    printf("%-22s %8s %12s %12s %12s\n", "pass", "time(s)", "minor", "major", "ns/page");
    for (int mode = 0; mode < 2; mode++) {
        if (mode) { munmap(a, n * ps); a = mmap(NULL, n * ps, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); }
        for (int p = 0; p < passes; p++) {
            long m0, j0, m1, j1; faults(&m0, &j0); double t0 = now();
            for (size_t i = 0; i < n; i++) a[(mode ? order[i] : i) * ps] += 1;
            double t = now() - t0; faults(&m1, &j1);
            char l[48]; snprintf(l, sizeof l, "%s pass %d%s", mode ? "random    " : "sequential", p + 1, p == 0 ? " (first touch)" : "");
            printf("%-22s %8.4f %12ld %12ld %12.1f\n", l, t, m1 - m0, j1 - j0, t / n * 1e9);
        }
    }
    return 0;
}
