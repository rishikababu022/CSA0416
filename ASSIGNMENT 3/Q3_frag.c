/* Q3: external fragmentation of segmentation (20 random segments, remove 10, load new ones) and
 * table-size comparison of pure paging, pure segmentation and segmented paging. */
#include <stdio.h>
#include <stdlib.h>
#define MEM 1024      /* KB */
typedef struct { int start, size, used; } Seg;
Seg seg[20]; char map[MEM];

static int first_fit(int size)
{
    int run = 0;
    for (int i = 0; i < MEM; i++) { run = map[i] ? 0 : run + 1; if (run == size) return i - size + 1; }
    return -1;
}
static void report(const char *t)
{
    int free_total = 0, holes = 0, largest = 0, run = 0;
    for (int i = 0; i <= MEM; i++) {
        if (i < MEM && !map[i]) { free_total++; run++; }
        else if (run) { holes++; if (run > largest) largest = run; run = 0; }
    }
    printf("%-34s free %4d KB in %2d holes, largest hole %4d KB, external fragmentation = 1 - largest/free = %.1f%%\n",
           t, free_total, holes, largest, free_total ? 100.0 * (1 - (double)largest / free_total) : 0.0);
}
int main(void)
{
    srand(7);
    for (int i = 0; i < 20; i++) {
        seg[i].size = 10 + rand() % 70;
        int s = first_fit(seg[i].size);
        if (s < 0) { seg[i].used = 0; continue; }
        seg[i].start = s; seg[i].used = 1; for (int k = 0; k < seg[i].size; k++) map[s + k] = 1;
    }
    report("after loading 20 segments:");
    int removed = 0;
    for (int i = 0; i < 20; i += 2) if (seg[i].used) { for (int k = 0; k < seg[i].size; k++) map[seg[i].start + k] = 0; seg[i].used = 0; removed++; }
    report("after removing 10 (every other):");
    int big = 0; for (int i = 0; i < 20; i++) if (!seg[i].used) big += 0;
    int want = 120, s = first_fit(want);
    printf("try to load one new 120 KB segment: %s\n", s >= 0 ? "loaded" : "FAILS although total free memory is larger (external fragmentation)");
    (void)big; (void)removed;

    printf("\n== Table sizes for a program: code 200 KB, data 600 KB, stack 100 KB (4 KB pages, 4 B entries, 8 B segment entries) ==\n");
    int pages = (200 + 600 + 100) / 4;
    printf("pure paging, flat 32-bit table      : 2^20 x 4 B = 4096 KB per process (a 2-level table needs only a 4 KB directory + %d page-table page(s) of 4 KB = %d KB)\n", (pages + 1023) / 1024, 4 + 4 * ((pages + 1023) / 1024));
    printf("pure segmentation                   : 3 segments x 8 B = 24 B (but segments need contiguous memory)\n");
    printf("segmented paging (page table/segment): 24 B + (%d + %d + %d) page entries x 4 B = %d B\n", 200 / 4, 600 / 4, 100 / 4, 24 + (50 + 150 + 25) * 4);
    printf("average internal fragmentation of segmented paging: about half a page per segment = 3 x 2 KB = 6 KB\n");
    return 0;
}
