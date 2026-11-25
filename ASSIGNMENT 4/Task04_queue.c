/* Task 4.1 and the LOOK / C-LOOK alternative: order of service and total head movement
 * for the queue 98 183 37 122 14 124 65 67, head at 53, cylinders 0-199. */
#include <stdio.h>
#include <stdlib.h>
#define N 8
#define MAXCYL 199

static const int req[N] = {98, 183, 37, 122, 14, 124, 65, 67};
static int cmp(const void *a, const void *b) { return *(const int *)a - *(const int *)b; }

static void report(const char *name, const int *stop, int n, int head, int jump)
{
    int pos = head, total = 0;
    printf("%-12s %d", name, head);
    for (int i = 0; i < n; i++) { printf(" %d", stop[i]); total += abs(stop[i] - pos); pos = stop[i]; }
    printf("  -> %d cylinders", total);
    if (jump) printf(" (%d if the return jump of %d is not counted)", total - jump, jump);
    printf("\n");
}

int main(void)
{
    int head = 53, s[N], o[N + 3], n, i;
    for (i = 0; i < N; i++) s[i] = req[i];
    qsort(s, N, sizeof s[0], cmp);

    report("FCFS", req, N, head, 0);

    int done[N] = {0}, pos = head;
    for (int k = 0; k < N; k++) {
        int best = -1;
        for (int j = 0; j < N; j++)
            if (!done[j] && (best < 0 || abs(req[j] - pos) < abs(req[best] - pos))) best = j;
        done[best] = 1; o[k] = req[best]; pos = req[best];
    }
    report("SSTF", o, N, head, 0);

    n = 0;                                                  /* SCAN, towards 0 first */
    for (i = N - 1; i >= 0; i--) if (s[i] < head) o[n++] = s[i];
    o[n++] = 0;
    for (i = 0; i < N; i++) if (s[i] > head) o[n++] = s[i];
    report("SCAN(down)", o, n, head, 0);

    n = 0;                                                  /* C-SCAN, upward, jump 199 -> 0 */
    for (i = 0; i < N; i++) if (s[i] > head) o[n++] = s[i];
    o[n++] = MAXCYL; o[n++] = 0;
    for (i = 0; i < N; i++) if (s[i] < head) o[n++] = s[i];
    report("C-SCAN", o, n, head, MAXCYL);

    n = 0;                                                  /* LOOK, towards 0 first */
    for (i = N - 1; i >= 0; i--) if (s[i] < head) o[n++] = s[i];
    for (i = 0; i < N; i++) if (s[i] > head) o[n++] = s[i];
    report("LOOK(down)", o, n, head, 0);

    n = 0;                                                  /* LOOK, towards 199 first */
    for (i = 0; i < N; i++) if (s[i] > head) o[n++] = s[i];
    for (i = N - 1; i >= 0; i--) if (s[i] < head) o[n++] = s[i];
    report("LOOK(up)", o, n, head, 0);

    n = 0;                                                  /* C-LOOK: up, jump to lowest request */
    int above = 0;
    for (i = 0; i < N; i++) if (s[i] > head) { o[n++] = s[i]; above++; }
    for (i = 0; i < N; i++) if (s[i] < head) o[n++] = s[i];
    report("C-LOOK", o, n, head, o[above - 1] - o[above]);
    return 0;
}
