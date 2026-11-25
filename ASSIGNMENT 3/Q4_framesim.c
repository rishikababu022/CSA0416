/* Page faults against number of frames (FIFO and LRU) for a reference string -> Q4_faults_vs_frames.csv */
#include <stdio.h>
#define MAXF 16
static int find(const int f[], int n, int p) { for (int i = 0; i < n; i++) if (f[i] == p) return i; return -1; }
static int fifo(const int *r, int len, int fr)
{ int f[MAXF], used = 0, next = 0, faults = 0;
  for (int i = 0; i < len; i++) { if (find(f, used, r[i]) >= 0) continue; faults++; if (used < fr) f[used++] = r[i]; else { f[next] = r[i]; next = (next + 1) % fr; } }
  return faults; }
static int lru(const int *r, int len, int fr)
{ int f[MAXF], last[MAXF], used = 0, faults = 0;
  for (int i = 0; i < len; i++) { int k = find(f, used, r[i]); if (k >= 0) { last[k] = i; continue; } faults++;
    if (used < fr) { f[used] = r[i]; last[used++] = i; } else { int v = 0; for (int j = 1; j < fr; j++) if (last[j] < last[v]) v = j; f[v] = r[i]; last[v] = i; } }
  return faults; }
static int opt(const int *r, int len, int fr)
{ int f[MAXF], used = 0, faults = 0;
  for (int i = 0; i < len; i++) { if (find(f, used, r[i]) >= 0) continue; faults++; if (used < fr) { f[used++] = r[i]; continue; }
    int v = 0, far = -1; for (int j = 0; j < fr; j++) { int nx = len; for (int t = i + 1; t < len; t++) if (r[t] == f[j]) { nx = t; break; } if (nx > far) { far = nx; v = j; } }
    f[v] = r[i]; }
  return faults; }
int main(void)
{
    int ref[] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1}; int len = 20;
    FILE *c = fopen("Q4_faults_vs_frames.csv", "w"); fprintf(c, "frames,FIFO,LRU,Optimal\n");
    printf("reference string: 7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1 (20 references, 8 distinct pages)\nframes  FIFO  LRU  OPT\n");
    for (int fr = 1; fr <= 8; fr++) { int a = fifo(ref, len, fr), b = lru(ref, len, fr), o = opt(ref, len, fr); printf("%6d %5d %4d %4d\n", fr, a, b, o); fprintf(c, "%d,%d,%d,%d\n", fr, a, b, o); }
    fclose(c); printf("written Q4_faults_vs_frames.csv\n"); return 0;
}
