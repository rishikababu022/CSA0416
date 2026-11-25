/* Q7 Stage 3: time-critical loop with and without locked memory; worst-case delay measurement.
 * Usage: ./Q7_rt_lock nolock | lock [ITER]
 * The loop touches one page of a large buffer per iteration. Without locking, the first touch of each
 * page (or a page that was reclaimed) causes a page fault inside the loop -> a long delay.
 * With mlockall(MCL_CURRENT|MCL_FUTURE) all pages are made resident BEFORE the loop starts.
 * mlockall needs privilege or a high RLIMIT_MEMLOCK: run as root, or `ulimit -l unlimited`. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/resource.h>

static long long ns(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec * 1000000000LL + t.tv_nsec; }
static int cmp(const void *a, const void *b) { long long x = *(const long long *)a, y = *(const long long *)b; return (x > y) - (x < y); }

int main(int argc, char **argv)
{
    int lock = argc > 1 && !strcmp(argv[1], "lock");
    long iters = argc > 2 ? atol(argv[2]) : 20000;
    long ps = sysconf(_SC_PAGESIZE); size_t pages = 16384;           /* 64 MB working buffer */
    if (lock && mlockall(MCL_CURRENT | MCL_FUTURE) != 0) { perror("mlockall (try root or ulimit -l unlimited)"); return 1; }
    char *buf = mmap(NULL, pages * ps, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);   /* populated now if locked */
    if (buf == MAP_FAILED) { perror("mmap"); return 1; }
    long long *lat = malloc(iters * sizeof *lat);
    if (lock) memset(lat, 0, iters * sizeof *lat);
    struct rusage a, b; getrusage(RUSAGE_SELF, &a);
    for (long i = 0; i < iters; i++) {
        long long t0 = ns();
        buf[(size_t)(i % pages) * ps] += 1;                           /* the "real-time work": one memory access */
        lat[i] = ns() - t0;
    }
    getrusage(RUSAGE_SELF, &b);
    long long mx = 0, sum = 0; for (long i = 0; i < iters; i++) { sum += lat[i]; if (lat[i] > mx) mx = lat[i]; }
    qsort(lat, iters, sizeof *lat, cmp);
    printf("%-7s iterations %ld: average %lld ns, median %lld ns, 99.9%% %lld ns, WORST CASE %lld ns (%.1f us), page faults inside loop: %ld minor, %ld major\n",
           lock ? "LOCKED" : "NOLOCK", iters, sum / iters, lat[iters / 2], lat[(long)(iters * 0.999)], mx, mx / 1000.0,
           b.ru_minflt - a.ru_minflt, b.ru_majflt - a.ru_majflt);
    return 0;
}
