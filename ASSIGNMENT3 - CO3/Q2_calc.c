/* Q2 calculations and experiments: page-table size, EAT, internal fragmentation,
 * system page size, page faults, boundary address tests. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/resource.h>

static void table_sizes(void)
{
    printf("== Page-table size (4 KB pages, single-level table) ==\n");
    double e32 = pow(2, 32 - 12), e48 = pow(2, 48 - 12);
    printf("32-bit: %.0f entries x 4 B  = %.0f bytes = %.1f MB per process\n", e32, e32 * 4, e32 * 4 / (1 << 20));
    printf("48-bit: %.0f entries x 8 B  = %.0f bytes = %.0f GB per process (why multi-level/hashed tables are needed)\n",
           e48, e48 * 8, e48 * 8 / pow(2, 30));
}

static void eat(void)
{
    double mem = 100, tlb = 20;
    printf("\n== Effective access time (memory 100 ns, TLB 20 ns) ==\n");
    printf("Without TLB (single-level table): 2 x 100 = %.0f ns\n", 2 * mem);
    double hits[] = {0.80, 0.95, 0.99};
    for (int i = 0; i < 3; i++) {
        double h = hits[i];
        double e = h * (tlb + mem) + (1 - h) * (tlb + 2 * mem);
        printf("TLB hit ratio %2.0f%%: EAT = %.2f*%.0f + %.2f*%.0f = %.1f ns (slow-down vs 100 ns: %.1f%%)\n",
               h * 100, h, tlb + mem, 1 - h, tlb + 2 * mem, e, (e - mem) / mem * 100);
    }
}

static void internal_frag(void)
{
    printf("\n== Internal fragmentation, 100 random processes (10 KB - 50 MB, seed 42) ==\n");
    srand(42);
    long size[100]; double total = 0;
    for (int i = 0; i < 100; i++) { size[i] = (10L << 10) + (long)((double)rand() / RAND_MAX * ((50L << 20) - (10L << 10))); total += size[i]; }
    long pages[] = {1 << 10, 4 << 10, 2 << 20};
    const char *name[] = {"1 KB", "4 KB", "2 MB"};
    printf("total memory requested = %.1f MB\n", total / (1 << 20));
    for (int k = 0; k < 3; k++) {
        double waste = 0, npages = 0;
        for (int i = 0; i < 100; i++) {
            long n = (size[i] + pages[k] - 1) / pages[k];
            waste += n * pages[k] - size[i]; npages += n;
        }
        printf("page %-4s: pages used %9.0f, internal fragmentation total %10.1f KB, average %8.2f KB/process (%.4f%% of request); theory ~ page/2 = %.1f KB\n",
               name[k], npages, waste / 1024, waste / 1024 / 100, waste / total * 100, pages[k] / 2048.0);
    }
}

static void system_info(void)
{
    printf("\n== This system ==\n");
    printf("page size (sysconf) = %ld bytes  (same as: getconf PAGESIZE)\n", sysconf(_SC_PAGESIZE));
    long ps = sysconf(_SC_PAGESIZE); size_t n = 1000;
    struct rusage a, b; getrusage(RUSAGE_SELF, &a);
    char *p = mmap(NULL, n * ps, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    for (size_t i = 0; i < n; i++) p[i * ps] = 1;
    getrusage(RUSAGE_SELF, &b);
    printf("touching %zu fresh pages caused %ld minor page faults (major: %ld)\n", n, b.ru_minflt - a.ru_minflt, b.ru_majflt - a.ru_majflt);
}

static void boundary(void)
{
    printf("\n== Boundary tests: 8-page logical space, 4 KB pages, table {5,2,-1,7,0,-1,3,-1} ==\n");
    int pt[8] = {5, 2, -1, 7, 0, -1, 3, -1};
    unsigned P = 4096, tests[] = {0, 4095, 4096, 8191, 8192, 7 * 4096, 8 * 4096 - 1, 8 * 4096, 8 * 4096 + 100};
    const char *why[] = {"first byte of page 0", "last byte of page 0", "first byte of page 1", "last byte of page 1",
                         "page 2 not resident", "first byte of last page (7, not resident)", "last valid address",
                         "first address beyond last page", "far beyond last page"};
    for (int i = 0; i < 9; i++) {
        unsigned a = tests[i], p = a / P, d = a % P;
        if (p >= 8) printf("%6u (%s): page %u -> INVALID ADDRESS (trap, segmentation fault)\n", a, why[i], p);
        else if (pt[p] < 0) printf("%6u (%s): page %u offset %u -> PAGE FAULT\n", a, why[i], p, d);
        else printf("%6u (%s): page %u offset %u -> frame %d, physical %u\n", a, why[i], p, d, pt[p], pt[p] * P + d);
    }
}

int main(void)
{
    table_sizes(); eat(); internal_frag(); system_info(); boundary();
    return 0;
}
