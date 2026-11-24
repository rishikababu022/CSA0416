/* Task 1.1 / 1.2: cost of finding a file among 12,000 students.
 *   flat      : one directory, linear scan                    O(n)
 *   tree      : /home/<dept>/<year>/<regno>/ - linear scan inside each small directory
 *   hashed    : hashed directory (ext4 htree / NTFS B-tree like)  O(1) / O(log n)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 12000
#define LOOKUPS 20000
#define DEPTS 10
#define YEARS 4
#define HSIZE 32768

static char names[N][16];
static int dept[N], year[N];
static int table[HSIZE];

static unsigned hash(const char *s) { unsigned h = 5381; while (*s) h = h * 33 + (unsigned char)*s++; return h; }

int main(void)
{
    srand(7);
    for (int i = 0; i < N; i++) {
        snprintf(names[i], sizeof names[i], "REG%05d", i);
        dept[i] = i % DEPTS; year[i] = (i / DEPTS) % YEARS;
    }
    for (int i = 0; i < HSIZE; i++) table[i] = -1;
    for (int i = 0; i < N; i++) { unsigned h = hash(names[i]) % HSIZE; while (table[h] >= 0) h = (h + 1) % HSIZE; table[h] = i; }

    int q[LOOKUPS];
    for (int i = 0; i < LOOKUPS; i++) q[i] = rand() % N;
    long cmp_flat = 0, cmp_tree = 0, cmp_hash = 0;
    volatile int sink = 0;

    clock_t t0 = clock();
    for (int i = 0; i < LOOKUPS; i++)
        for (int j = 0; j < N; j++) { cmp_flat++; if (strcmp(names[j], names[q[i]]) == 0) { sink = j; break; } }
    double t_flat = (double)(clock() - t0) / CLOCKS_PER_SEC;

    t0 = clock();
    for (int i = 0; i < LOOKUPS; i++) {
        int d = dept[q[i]], y = year[q[i]];
        cmp_tree += d + 1 + y + 1;                         /* walk dept and year directories */
        for (int j = 0; j < N; j++) {
            if (dept[j] != d || year[j] != y) continue;    /* only entries of that small directory */
            cmp_tree++;
            if (strcmp(names[j], names[q[i]]) == 0) { sink = j; break; }
        }
    }
    double t_tree = (double)(clock() - t0) / CLOCKS_PER_SEC;

    t0 = clock();
    for (int i = 0; i < LOOKUPS; i++) {
        unsigned h = hash(names[q[i]]) % HSIZE;
        while (table[h] >= 0) { cmp_hash++; if (strcmp(names[table[h]], names[q[i]]) == 0) { sink = table[h]; break; } h = (h + 1) % HSIZE; }
    }
    double t_hash = (double)(clock() - t0) / CLOCKS_PER_SEC;
    (void)sink;

    printf("%d students, %d random look-ups\n", N, LOOKUPS);
    printf("%-34s %12s %14s\n", "organisation", "avg compares", "total time (s)");
    printf("%-34s %12.1f %14.3f\n", "flat directory, linear scan", (double)cmp_flat / LOOKUPS, t_flat);
    printf("%-34s %12.1f %14.3f\n", "dept/year tree (approx. compares)", (double)cmp_tree / LOOKUPS, t_tree);
    printf("%-34s %12.1f %14.3f\n", "hashed directory", (double)cmp_hash / LOOKUPS, t_hash);
    printf("(the tree timing includes the filter loop used to model small directories, so use the compare counts)\n");
    return 0;
}
