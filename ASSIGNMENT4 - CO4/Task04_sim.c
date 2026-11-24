/* Task 4.2: simulation of FCFS, SSTF, SCAN, C-SCAN, LOOK, C-LOOK with 10,000 random
 * requests under light, medium and heavy load.
 *
 * Model (stated assumptions):
 *   - cylinders 0..199, uniform random request cylinder, head starts at 100
 *   - Poisson arrivals; mean inter-arrival time: light 40 ms, medium 10 ms, heavy 6 ms
 *   - service time = 0.1 ms per cylinder of seek + 1.0 ms rotation/transfer
 *   - SCAN/C-SCAN go to the physical end of the disk; LOOK/C-LOOK turn at the last request
 *   - C-SCAN / C-LOOK return jump is counted as head movement and takes 0.1 ms per cylinder
 *   - wait = time from arrival until the head starts moving to the request
 *     (so sweeps to the disk end before the request are part of its wait)
 * The same request stream is used for every algorithm in a given load.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NREQ 10000
#define MAXCYL 199
#define SEEK_MS 0.1
#define XFER_MS 1.0

enum { FCFS, SSTF, SCAN, CSCAN, LOOK, CLOOK, NALG };
static const char *names[NALG] = {"FCFS", "SSTF", "SCAN", "C-SCAN", "LOOK", "C-LOOK"};

typedef struct { double arr; int cyl; } Req;
static Req rq[NREQ];
static int pend[NREQ];

static unsigned long long rs = 88172645463325252ULL;
static double urand(void)
{
    rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17;
    return (double)(rs >> 11) * (1.0 / 9007199254740992.0);
}

static void generate(double mean_ia)
{
    double t = 0;
    for (int i = 0; i < NREQ; i++) {
        t += -mean_ia * log(1.0 - urand());
        rq[i].arr = t;
        rq[i].cyl = (int)(urand() * (MAXCYL + 1));
    }
}

static void run(int alg, double *avg_move, double *avg_wait, double *max_wait)
{
    int np = 0, next = 0, served = 0, head = 100, dir = 1;
    double t = 0, moved = 0, sumw = 0, maxw = 0;

    while (served < NREQ) {
        if (np == 0 && next < NREQ && rq[next].arr > t) t = rq[next].arr;
        while (next < NREQ && rq[next].arr <= t) pend[np++] = next++;

        int pick = -1;
        for (;;) {
            pick = -1;
            if (alg == FCFS) {
                pick = 0;
            } else if (alg == SSTF) {
                for (int k = 0; k < np; k++)
                    if (pick < 0 || abs(rq[pend[k]].cyl - head) < abs(rq[pend[pick]].cyl - head)) pick = k;
            } else if (alg == SCAN || alg == LOOK) {
                for (int k = 0; k < np; k++) {
                    int c = rq[pend[k]].cyl;
                    if (dir > 0 && c >= head && (pick < 0 || c < rq[pend[pick]].cyl)) pick = k;
                    if (dir < 0 && c <= head && (pick < 0 || c > rq[pend[pick]].cyl)) pick = k;
                }
            } else { /* CSCAN, CLOOK: always upward */
                for (int k = 0; k < np; k++) {
                    int c = rq[pend[k]].cyl;
                    if (c >= head && (pick < 0 || c < rq[pend[pick]].cyl)) pick = k;
                }
            }
            if (pick >= 0) break;

            /* nothing ahead of the head */
            if (alg == SCAN) {
                int edge = dir > 0 ? MAXCYL : 0;
                int d = abs(edge - head);
                moved += d; t += d * SEEK_MS; head = edge; dir = -dir;
            } else if (alg == LOOK) {
                dir = -dir;
            } else if (alg == CSCAN) {
                int d = (MAXCYL - head) + MAXCYL;          /* to the end, then jump to 0 */
                moved += d; t += d * SEEK_MS; head = 0;
            } else if (alg == CLOOK) {
                int low = MAXCYL + 1;
                for (int k = 0; k < np; k++) if (rq[pend[k]].cyl < low) low = rq[pend[k]].cyl;
                int d = head - low;                          /* jump back to the lowest request */
                moved += d; t += d * SEEK_MS; head = low;
            }
            while (next < NREQ && rq[next].arr <= t) pend[np++] = next++;
        }

        Req *r = &rq[pend[pick]];
        double wait = t - r->arr;
        sumw += wait;
        if (wait > maxw) maxw = wait;
        int d = abs(r->cyl - head);
        moved += d;
        t += d * SEEK_MS + XFER_MS;
        head = r->cyl;
        for (int k = pick; k < np - 1; k++) pend[k] = pend[k + 1];   /* keep arrival order */
        np--;
        served++;
    }
    *avg_move = moved / NREQ;
    *avg_wait = sumw / NREQ;
    *max_wait = maxw;
}

int main(void)
{
    const char *load_name[3] = {"light", "medium", "heavy"};
    const double mean_ia[3] = {40.0, 10.0, 6.0};

    printf("Disk scheduling simulation: %d random requests, cylinders 0-%d\n", NREQ, MAXCYL);
    printf("service = %.1f ms/cyl seek + %.1f ms transfer\n\n", SEEK_MS, XFER_MS);
    for (int l = 0; l < 3; l++) {
        rs = 88172645463325252ULL + (unsigned long long)l * 7919ULL;
        generate(mean_ia[l]);
        printf("Load: %s (mean inter-arrival %.0f ms)\n", load_name[l], mean_ia[l]);
        printf("  %-7s %18s %16s %16s\n", "algo", "avg move (cyl/req)", "avg wait (ms)", "max wait (ms)");
        for (int a = 0; a < NALG; a++) {
            double am, aw, mw;
            run(a, &am, &aw, &mw);
            printf("  %-7s %18.2f %16.1f %16.1f\n", names[a], am, aw, mw);
        }
        printf("\n");
    }
    return 0;
}
