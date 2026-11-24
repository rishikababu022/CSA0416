/* Task 4 "Complete the Code" follow-up: a stream of requests near cylinder 60 arriving
 * every 1 ms. What happens to the request at cylinder 183 (head at 53, queue as given)?
 *
 * Service time = 0.1 ms per cylinder + 1.0 ms transfer (>= 1 ms), so with one new
 * request per millisecond the queue near cylinder 60 never empties.
 */
#include <stdio.h>
#include <stdlib.h>

#define MAXP 60000
#define MAXCYL 199
#define HORIZON 20000.0
#define SEEK_MS 0.1
#define XFER_MS 1.0

static int cyl[MAXP];
static unsigned rstate = 12345u;
static int rnd(int n) { rstate = rstate * 1103515245u + 12345u; return (int)((rstate >> 16) % (unsigned)n); }

static void run(const char *name, int alg) /* 0 = SSTF, 1 = SCAN (down first), 2 = LOOK (down first) */
{
    const int init[8] = {98, 183, 37, 122, 14, 124, 65, 67};
    int np = 0, head = 53, dir = -1, served = 0, found = 0;
    double t = 0, nextarr = 1.0;
    for (int i = 0; i < 8; i++) cyl[np++] = init[i];
    rstate = 12345u;

    while (t < HORIZON && !found) {
        while (nextarr <= t) { cyl[np++] = 58 + rnd(5); nextarr += 1.0; }
        if (np == 0) { t = nextarr; continue; }
        int pick = -1;
        if (alg == 0) {
            for (int k = 0; k < np; k++)
                if (pick < 0 || abs(cyl[k] - head) < abs(cyl[pick] - head)) pick = k;
        } else {
            for (int k = 0; k < np; k++) {
                if (dir < 0 && cyl[k] <= head && (pick < 0 || cyl[k] > cyl[pick])) pick = k;
                if (dir > 0 && cyl[k] >= head && (pick < 0 || cyl[k] < cyl[pick])) pick = k;
            }
            if (pick < 0) {
                if (alg == 1) { int edge = dir < 0 ? 0 : MAXCYL; t += abs(edge - head) * SEEK_MS; head = edge; }
                dir = -dir;
                continue;
            }
        }
        t += abs(cyl[pick] - head) * SEEK_MS + XFER_MS;
        head = cyl[pick];
        if (cyl[pick] == 183) found = 1;
        cyl[pick] = cyl[--np];
        served++;
    }
    if (found) printf("%-5s: request at 183 served at t = %.1f ms (after %d other requests)\n", name, t, served - 1);
    else printf("%-5s: request at 183 NOT served in %.0f ms; %d requests served, %d still queued "
                "(183 has waited %.0f ms) -> starvation\n", name, HORIZON, served, np, t);
}

int main(void)
{
    run("SSTF", 0);
    run("SCAN", 1);
    run("LOOK", 2);
    return 0;
}
