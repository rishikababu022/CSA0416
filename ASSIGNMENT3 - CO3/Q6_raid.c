/* Q6: RAID capacity, write cost, fault tolerance and probability of data loss (6 disks x 4 TB). */
#include <stdio.h>
#include <math.h>
static double C(int n, int k) { double r = 1; for (int i = 1; i <= k; i++) r = r * (n - k + i) / i; return r; }
int main(void)
{
    int n = 6; double tb = 4;
    printf("== Usable capacity of %d disks of %.0f TB (raw %.0f TB) ==\n", n, tb, n * tb);
    printf("RAID 0  : %2.0f TB (100%%)   survives 0 failures   write cost 1 I/O   read speed ~%dx\n", n * tb, n);
    printf("RAID 1  : %2.0f TB (50%% as 3 mirrored pairs; one 6-way mirror would give only 4 TB = 17%%)\n", n * tb / 2);
    printf("          pairs: survives 1 failure (2 only if in different pairs), write cost 2 I/Os\n");
    printf("RAID 5  : %2.0f TB (%.0f%%)   survives 1 failure   write cost 4 I/Os (read old data+parity, write new data+parity)\n", (n - 1) * tb, 100.0 * (n - 1) / n);
    printf("RAID 6  : %2.0f TB (%.0f%%)   survives 2 failures  write cost 6 I/Os (two parities, extra parity calculation)\n", (n - 2) * tb, 100.0 * (n - 2) / n);
    printf("RAID 10 : %2.0f TB (50%%)    survives 1 failure guaranteed, up to 3 (one per mirror pair); write cost 2 I/Os\n", n * tb / 2);

    double afr = 0.03, rebuild_h = 24;                 /* assumptions: 3%% annual failure rate, 24 h rebuild */
    double p = 1 - pow(1 - afr, rebuild_h / 8760.0);   /* probability one given disk fails during rebuild window */
    printf("\n== Probability of data loss after one disk has failed ==\n");
    printf("assumptions: annual failure rate %.1f%%, rebuild time %.0f h -> p(a given disk fails during rebuild) = %.5f%%\n", afr * 100, rebuild_h, p * 100);
    double r1 = p, r5 = 1 - pow(1 - p, n - 1);
    double r6 = 0; for (int k = 2; k <= n - 1; k++) r6 += C(n - 1, k) * pow(p, k) * pow(1 - p, n - 1 - k);
    printf("RAID 1 (mirror of the failed disk must survive)   : %.5f%%\n", r1 * 100);
    printf("RAID 10 (same as RAID 1: only the partner matters) : %.5f%%\n", r1 * 100);
    printf("RAID 5 (any of the %d other disks failing)         : %.5f%%\n", n - 1, r5 * 100);
    printf("RAID 6 (two more of the %d remaining must fail)    : %.8f%%\n", n - 1, r6 * 100);
    double ber = 1e-14, bits = (n - 1) * tb * 1e12 * 8;
    printf("\nUnrecoverable read error during rebuild (1 error per 1e14 bits), RAID 5 must read %.0f TB = %.2e bits:\n", (n - 1) * tb, bits);
    printf("P(at least one URE) = 1-(1-1e-14)^bits = %.1f%%  -> large RAID 5 arrays of big disks are risky; RAID 6/10 are safer\n", (1 - exp(-ber * bits)) * 100);
    printf("RAID 1/10 rebuild reads only %.0f TB: P(URE) = %.1f%%\n", tb, (1 - exp(-ber * tb * 1e12 * 8)) * 100);
    return 0;
}
