/* Q4: effective access time with page faults, maximum fault rate for 10% slow-down, frame allocation. */
#include <stdio.h>
int main(void)
{
    double mem = 100, svc = 8e6;   /* ns */
    double rates[] = {1e-3, 1e-5, 1e-6};
    const char *lab[] = {"1 in 1,000", "1 in 100,000", "1 in 1,000,000"};
    printf("== Effective access time: EAT = (1-p)*100 + p*8,000,000 ns ==\n");
    for (int i = 0; i < 3; i++) {
        double e = (1 - rates[i]) * mem + rates[i] * svc;
        printf("p = %-15s EAT = %10.2f ns   slow-down = %9.1f%%  (%.1fx)\n", lab[i], e, (e - mem) / mem * 100, e / mem);
    }
    double pmax = (0.10 * mem) / (svc - mem);
    printf("\nMax page-fault rate for a 10%% slow-down (EAT <= 110 ns): p <= 10 / (8,000,000 - 100) = %.3e = 1 fault per %.0f accesses\n", pmax, 1 / pmax);
    printf("\n== Frame allocation: 62 free frames, processes of 10 and 127 pages ==\n");
    int s1 = 10, s2 = 127, m = 62, S = s1 + s2;
    printf("equal allocation        : %d and %d frames\n", m / 2, m / 2);
    printf("proportional allocation : %d x 62 / %d = %.2f -> %d and %d x 62 / %d = %.2f -> %d frames\n", s1, S, (double)s1 * m / S, s1 * m / S, s2, S, (double)s2 * m / S, s2 * m / S);
    printf("\n== Alternative: 32 GB RAM, no paging to disk (rate measured by Q4_touch is used in the README) ==\n");
    printf("EAT with no faults = 100 ns; with p=1e-5: %.1f ns; cost of extra RAM must be compared with the run-time saving.\n", (1 - 1e-5) * mem + 1e-5 * svc);
    return 0;
}
