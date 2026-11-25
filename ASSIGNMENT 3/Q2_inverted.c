/* Alternative concept: inverted page table (one entry per physical frame) with a hash chain.
 * Compares space and look-up cost with a per-process multi-level page table. */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define FRAMES 64
#define HASH 32
typedef struct { int pid, page, next; } Entry;
Entry ipt[FRAMES]; int bucket[HASH]; int nfree = 0;

static unsigned h(int pid, int page) { return ((unsigned)pid * 31u + (unsigned)page) % HASH; }
static int insert(int pid, int page)
{
    if (nfree >= FRAMES) return -1;
    int f = nfree++; ipt[f].pid = pid; ipt[f].page = page;
    unsigned b = h(pid, page); ipt[f].next = bucket[b]; bucket[b] = f; return f;
}
static int lookup(int pid, int page, int *probes)
{
    *probes = 0;
    for (int f = bucket[h(pid, page)]; f != -1; f = ipt[f].next) { (*probes)++; if (ipt[f].pid == pid && ipt[f].page == page) return f; }
    return -1;
}
int main(void)
{
    for (int i = 0; i < HASH; i++) bucket[i] = -1;
    srand(1);
    for (int i = 0; i < FRAMES; i++) insert(1 + rand() % 5, rand() % 1000);
    int total = 0, hits = 0, maxp = 0;
    for (int f = 0; f < FRAMES; f++) { int pr, r = lookup(ipt[f].pid, ipt[f].page, &pr); if (r == f) hits++; total += pr; if (pr > maxp) maxp = pr; }
    printf("inverted table: %d frames, %d hash buckets, all %d resident pages found; average probes %.2f, max %d (O(1) expected)\n",
           FRAMES, HASH, hits, (double)total / FRAMES, maxp);
    int pr; int r = lookup(99, 5, &pr);
    printf("lookup of a non-resident page (pid 99): %s after %d probes -> page fault\n", r < 0 ? "not found" : "found", pr);

    printf("\n== Space comparison: 16 GB RAM, 4 KB pages, 100 processes each with 48-bit address space ==\n");
    double frames = 16.0 * pow(2, 30) / 4096;
    printf("inverted: %.0f entries x 16 B = %.0f MB total, independent of number of processes\n", frames, frames * 16 / pow(2, 20));
    printf("per-process 4-level table, 4 KB table pages, if a process maps 100 MB densely: about %.0f KB per process = %.1f MB for 100 processes (grows with processes)\n",
           (100.0 * 1024 / 4) * 8 / 1024 + 4 * 4, ((100.0 * 1024 / 4) * 8 / 1024 + 16) * 100 / 1024);
    printf("per-process worst case: the 2^36-entry flat table would need 512 GB each (impossible).\n");
    printf("Time: inverted = hash + chain walk (~1-2 memory accesses, but no sharing of frames between processes and slow lookup by frame);\n"
           "      4-level walk = 4 memory accesses on a TLB miss.\n");
    return 0;
}
