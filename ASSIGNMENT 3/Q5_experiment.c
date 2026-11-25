/* Q5 experiments: FIFO, LRU, Optimal and Clock on locality / random workloads, 3..10 frames,
 * timing for 100,000 references, and a step-by-step trace for hand verification.
 * Usage: ./Q5_experiment          (CSV + timing)
 *        ./Q5_experiment trace    (hand-calculation trace on the textbook string) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MAXF 16
#define N 100000

static int find(const int f[], int n, int p) { for (int i = 0; i < n; i++) if (f[i] == p) return i; return -1; }

static int fifo(const int *r, int len, int fr)
{ int f[MAXF], used = 0, next = 0, faults = 0;
  for (int i = 0; i < len; i++) { if (find(f, used, r[i]) >= 0) continue; faults++;
    if (used < fr) f[used++] = r[i]; else { f[next] = r[i]; next = (next + 1) % fr; } }
  return faults; }

static int lru(const int *r, int len, int fr)
{ int f[MAXF], last[MAXF], used = 0, faults = 0;
  for (int i = 0; i < len; i++) { int k = find(f, used, r[i]); if (k >= 0) { last[k] = i; continue; } faults++;
    if (used < fr) { f[used] = r[i]; last[used++] = i; }
    else { int v = 0; for (int j = 1; j < fr; j++) if (last[j] < last[v]) v = j; f[v] = r[i]; last[v] = i; } }
  return faults; }

/* Optimal in O(len*frames) using precomputed next-use positions */
static int optimal(const int *r, int len, int fr)
{ static int nxt[N + 1], lastpos[4096]; int f[MAXF], fnext[MAXF], used = 0, faults = 0;
  for (int i = 0; i < 4096; i++) lastpos[i] = len;
  for (int i = len - 1; i >= 0; i--) { nxt[i] = lastpos[r[i]]; lastpos[r[i]] = i; }
  for (int i = 0; i < len; i++) { int k = find(f, used, r[i]);
    if (k >= 0) { fnext[k] = nxt[i]; continue; } faults++;
    if (used < fr) { f[used] = r[i]; fnext[used++] = nxt[i]; }
    else { int v = 0; for (int j = 1; j < fr; j++) if (fnext[j] > fnext[v]) v = j; f[v] = r[i]; fnext[v] = nxt[i]; } }
  return faults; }

/* Clock (second chance): one reference bit per frame and a hand */
static int clock_alg(const int *r, int len, int fr)
{ int f[MAXF], bit[MAXF], used = 0, hand = 0, faults = 0;
  for (int i = 0; i < len; i++) { int k = find(f, used, r[i]); if (k >= 0) { bit[k] = 1; continue; } faults++;
    if (used < fr) { f[used] = r[i]; bit[used++] = 1; continue; }
    while (bit[hand]) { bit[hand] = 0; hand = (hand + 1) % fr; }
    f[hand] = r[i]; bit[hand] = 1; hand = (hand + 1) % fr; }
  return faults; }

static void gen_locality(int *r)
{ int base = 0, set = 8, space = 100;
  for (int i = 0; i < N; i++) {
    if (i % 2000 == 0) base = rand() % (space - set);          /* locality set moves */
    r[i] = (rand() % 100 < 90) ? base + rand() % set : rand() % space; } }
static void gen_random(int *r) { for (int i = 0; i < N; i++) r[i] = rand() % 100; }

static double secs(void) { struct timespec t; clock_gettime(CLOCK_MONOTONIC, &t); return t.tv_sec + t.tv_nsec * 1e-9; }

static void trace(const char *name, const int *r, int len, int fr, int use_lru)
{ int f[MAXF], last[MAXF], used = 0, next = 0, faults = 0;
  printf("\n%s, %d frames\n ref | frames      | fault\n", name, fr);
  for (int i = 0; i < len; i++) { int k = find(f, used, r[i]), flt = 0;
    if (k >= 0) { last[k] = i; }
    else { flt = 1; faults++;
      if (used < fr) { f[used] = r[i]; last[used++] = i; }
      else if (use_lru) { int v = 0; for (int j = 1; j < fr; j++) if (last[j] < last[v]) v = j; f[v] = r[i]; last[v] = i; }
      else { f[next] = r[i]; next = (next + 1) % fr; } }
    printf(" %3d | ", r[i]); for (int j = 0; j < fr; j++) { if (j < used) printf("%2d ", f[j]); else printf(" - "); }
    printf("   | %s\n", flt ? "F" : ""); }
  printf("total faults = %d\n", faults); }

int main(int argc, char **argv)
{
    if (argc > 1 && !strcmp(argv[1], "trace")) {
        int ref[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1};
        trace("FIFO", ref, 20, 3, 0); trace("LRU", ref, 20, 3, 1);
        return 0;
    }
    static int loc[N], rnd[N]; srand(2024); gen_locality(loc); gen_random(rnd);
    FILE *c = fopen("Q5_faults.csv", "w"); fprintf(c, "workload,frames,FIFO,LRU,Optimal,Clock\n");
    const char *wn[] = {"locality", "random"}; int *w[] = {loc, rnd};
    for (int k = 0; k < 2; k++) {
        printf("\nWorkload: %s (100,000 references, 100 distinct pages)\nframes   FIFO     LRU  Optimal   Clock   LRU-vs-Optimal  Clock-vs-LRU\n", wn[k]);
        for (int fr = 3; fr <= 10; fr++) {
            int a = fifo(w[k], N, fr), b = lru(w[k], N, fr), o = optimal(w[k], N, fr), cl = clock_alg(w[k], N, fr);
            printf("%6d %6d  %6d   %6d  %6d   %12.1f%%  %11.1f%%\n", fr, a, b, o, cl, 100.0 * (b - o) / o, 100.0 * (cl - b) / b);
            fprintf(c, "%s,%d,%d,%d,%d,%d\n", wn[k], fr, a, b, o, cl);
        }
    }
    fclose(c);
    printf("\nRunning time for 100,000 references (locality workload, 5 frames, average of 20 runs):\n");
    const char *an[] = {"FIFO", "LRU", "Optimal", "Clock"};
    for (int a = 0; a < 4; a++) {
        volatile int sink = 0; double t0 = secs();
        for (int rep = 0; rep < 20; rep++) sink += a == 0 ? fifo(loc, N, 5) : a == 1 ? lru(loc, N, 5) : a == 2 ? optimal(loc, N, 5) : clock_alg(loc, N, 5);
        printf("  %-8s %8.3f ms\n", an[a], (secs() - t0) / 20 * 1000); (void)sink;
    }
    printf("\nOptimal needs the whole future string (offline); LRU scans last-use stamps (O(frames)/fault); Clock needs one bit/frame.\n");
    return 0;
}
