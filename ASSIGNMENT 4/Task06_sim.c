/* Task 6 Stage 5: simulate "start / jump to any minute" requests for a lecture-video service
 * and compare two designs.
 *
 *   Design A : 8 hard disks only (video v lives on disk v % 8), no cache
 *   Design B : the same 8 hard disks + 2 SSDs used as a 1 TB LRU cache of 64 MB segments
 *
 * Library : 5,000 videos, 300 MB - 2 GB (about 5.7 TB), Zipf(0.9) popularity.
 * Request : pick a video by popularity, jump to a uniformly random 64 MB segment, read the
 *           4 MB start-up buffer. Latency = queueing delay + device service time.
 * Devices : HDD  4.2 ms rotation + 4..14 ms seek + 4 MB at 150 MB/s
 *           SSD  0.1 ms + 4 MB at 1500 MB/s
 * Each device is a FIFO server; requests arrive as a Poisson process (lambda per second).
 * Simplification: a missed segment is inserted into the cache when requested (not when the
 * HDD read completes) and the cache is not warmed before the run.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NVID 5000
#define MAXSEG 32
#define NKEY (NVID * MAXSEG)
#define NREQ 200000
#define NHDD 8
#define NSSD 2
#define SEG_MB 64.0
#define CHUNK_MB 4.0
#define SSD_CACHE_GB 1024

static unsigned long long rs;
static double urand(void)
{
    rs ^= rs << 13; rs ^= rs >> 7; rs ^= rs << 17;
    return (double)(rs >> 11) * (1.0 / 9007199254740992.0);
}

static int nseg[NVID];
static double cdf[NVID];
static double lat[NREQ];

static int lprev[NKEY], lnext[NKEY], lhead, ltail, lcount, lcap;
static char inc[NKEY];

static void lru_reset(int cap)
{
    for (int i = 0; i < NKEY; i++) inc[i] = 0;
    lhead = ltail = -1; lcount = 0; lcap = cap;
}
static void lru_unlink(int k)
{
    if (lprev[k] >= 0) lnext[lprev[k]] = lnext[k]; else lhead = lnext[k];
    if (lnext[k] >= 0) lprev[lnext[k]] = lprev[k]; else ltail = lprev[k];
}
static void lru_front(int k)
{
    lprev[k] = -1; lnext[k] = lhead;
    if (lhead >= 0) lprev[lhead] = k;
    lhead = k;
    if (ltail < 0) ltail = k;
}
static void lru_touch(int k)
{
    if (inc[k]) {
        if (lhead != k) { lru_unlink(k); lru_front(k); }
    } else {
        inc[k] = 1; lru_front(k); lcount++;
        if (lcount > lcap) { int v = ltail; lru_unlink(v); inc[v] = 0; lcount--; }
    }
}

static int pick_video(void)
{
    double u = urand();
    int lo = 0, hi = NVID - 1;
    while (lo < hi) { int mid = (lo + hi) / 2; if (cdf[mid] >= u) hi = mid; else lo = mid + 1; }
    return lo;
}

static int cmpd(const void *a, const void *b)
{
    double x = *(const double *)a, y = *(const double *)b;
    return (x > y) - (x < y);
}

static void simulate(int use_cache, double lambda)
{
    double hdd_busy[NHDD] = {0}, ssd_busy[NSSD] = {0}, hdd_work = 0, t = 0;
    long hits = 0;
    rs = 0x9E3779B97F4A7C15ULL;                       /* same request stream for both designs */
    lru_reset(SSD_CACHE_GB * 1024 / (int)SEG_MB);

    for (int i = 0; i < NREQ; i++) {
        t += -1000.0 / lambda * log(1.0 - urand());   /* ms */
        int v = pick_video();
        int seg = (int)(urand() * nseg[v]);
        int key = v * MAXSEG + seg;
        double svc, start, *busy;
        if (use_cache && inc[key]) {
            busy = &ssd_busy[key % NSSD];
            svc = 0.1 + CHUNK_MB / 1500.0 * 1000.0;
            hits++;
        } else {
            busy = &hdd_busy[v % NHDD];
            svc = 4.2 + 4.0 + 10.0 * urand() + CHUNK_MB / 150.0 * 1000.0;
            hdd_work += svc;
        }
        if (use_cache) lru_touch(key);
        start = t > *busy ? t : *busy;
        *busy = start + svc;
        lat[i] = *busy - t;
    }
    double sum = 0;
    for (int i = 0; i < NREQ; i++) sum += lat[i];
    double mean = sum / NREQ;
    qsort(lat, NREQ, sizeof lat[0], cmpd);
    printf("  %-22s lambda=%4.0f/s  mean %8.1f ms  p50 %8.1f  p95 %8.1f  p99 %8.1f  max %9.1f  "
           "SSD hit %5.1f%%  HDD util %5.1f%%\n",
           use_cache ? "B: 8 HDD + SSD cache" : "A: 8 HDD only", lambda, mean, lat[NREQ / 2],
           lat[(int)(NREQ * 0.95)], lat[(int)(NREQ * 0.99)], lat[NREQ - 1],
           100.0 * hits / NREQ, 100.0 * hdd_work / (t * NHDD));
}

int main(void)
{
    rs = 12345678901234567ULL;
    double total = 0, tot_mb = 0;
    for (int i = 0; i < NVID; i++) {
        double mb = 300.0 + urand() * 1748.0;
        nseg[i] = (int)ceil(mb / SEG_MB);
        tot_mb += mb;
        total += 1.0 / pow(i + 1.0, 0.9);
        cdf[i] = total;
    }
    for (int i = 0; i < NVID; i++) cdf[i] /= total;
    printf("Library: %d videos, %.2f TB; %d requests per run\n\n", NVID, tot_mb / 1e6, NREQ);
    const double lambdas[3] = {50, 100, 150};
    for (int l = 0; l < 3; l++) {
        simulate(0, lambdas[l]);
        simulate(1, lambdas[l]);
        printf("\n");
    }
    return 0;
}
