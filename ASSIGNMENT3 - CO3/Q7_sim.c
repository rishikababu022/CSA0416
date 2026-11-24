/* Q7 Stage 3: page-replacement simulation of the three tasks on a 2 GB edge device with NO swap.
 * Unit = 1 MB "page" (scaled model). Anonymous memory cannot be evicted (no swap), so only file-backed
 * pages (the ML model weights mapped from flash, log buffers) are replaceable. The control task's memory is locked
 * (mlock) and is never a victim. Global LRU over the unlocked pages.
 * Usage: ./Q7_sim [extra_task_MB]   e.g. ./Q7_sim 0   and   ./Q7_sim 500 */
#include <stdio.h>
#include <stdlib.h>
#define RAM 2048
#define OS_RESERVED 200
#define RT_MB 64
#define ML_MB 1536
#define LOG_MB 100
#define MAXP (RT_MB + ML_MB + LOG_MB + 4096)
static int resident[MAXP], locked[MAXP]; static long last[MAXP], faults[4], refs[4]; static int task_of[MAXP];

int main(int argc, char **argv)
{
    int extra = argc > 1 ? atoi(argv[1]) : 0;
    int total = RT_MB + ML_MB + LOG_MB + extra, base[4], size[4] = {RT_MB, ML_MB, LOG_MB, extra};
    for (int t = 0, b = 0; t < 4; t++) { base[t] = b; for (int i = 0; i < size[t]; i++) task_of[b + i] = t; b += size[t]; }
    int capacity = RAM - OS_RESERVED, used = 0;
    for (int i = 0; i < RT_MB; i++) { resident[i] = 1; locked[i] = 1; used++; }     /* mlockall: resident from the start */
    printf("RAM %d MB, OS reserved %d MB, usable %d MB, locked RT %d MB, demand of tasks %d MB (%s)\n", RAM, OS_RESERVED, capacity, RT_MB, total,
           total + 0 > capacity ? "EXCEEDS usable memory -> replacement of file-backed pages needed" : "fits");
    srand(5); long evictions = 0, N = 3000000;
    for (long now = 1; now <= N; now++) {
        int r = rand() % 100, t, p;
        if (r < 40) { t = 0; p = base[0] + rand() % RT_MB; }                                  /* control task */
        else if (r < 80) { t = 1; p = base[1] + ((rand() % 100 < 80) ? rand() % 900 : rand() % ML_MB); }  /* ML: hot 900 MB + scattered */
        else if (r < 90 || extra == 0) { t = 2; p = base[2] + (int)(now % LOG_MB); }       /* logger: sequential */
        else { t = 3; p = base[3] + rand() % extra; }
        refs[t]++;
        if (resident[p]) { last[p] = now; continue; }
        faults[t]++;
        if (used >= capacity) {                                                           /* evict LRU unlocked page */
            int v = -1; for (int i = 0; i < total; i++) if (resident[i] && !locked[i] && (v < 0 || last[i] < last[v])) v = i;
            if (v < 0) { printf("no victim available: out of memory (OOM kill)\n"); return 1; }
            resident[v] = 0; used--; evictions++;
        }
        resident[p] = 1; last[p] = now; used++;
    }
    const char *tn[] = {"control (RT, locked)", "machine learning", "logging", "extra task"};
    printf("%-22s %12s %12s %10s\n", "task", "references", "page faults", "fault rate");
    for (int t = 0; t < 4; t++) if (size[t]) printf("%-22s %12ld %12ld %9.4f%%\n", tn[t], refs[t], faults[t], refs[t] ? 100.0 * faults[t] / refs[t] : 0.0);
    printf("evictions: %ld\n", evictions);
    printf("%s\n", faults[0] == 0 ? "VERIFIED: the real-time task suffered 0 page faults." : "FAILED: real-time task had page faults!");
    return faults[0] != 0;
}
