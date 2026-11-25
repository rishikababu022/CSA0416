/* Task 3 alternative concept: write cost of small random 4 KiB updates on ext4 journaling
 * versus a copy-on-write file system (Btrfs/ZFS style). Analytical model - the formulas are
 * printed so that they can be compared with fio measurements on real ext4 and Btrfs volumes.
 *
 *   M = metadata blocks touched per update (inode + bitmap/extent block)        = 2
 *   D = depth of the copy-on-write tree whose path to the root is rewritten     = 3
 *   c = number of updates that share one metadata block in the same transaction (coalescing)
 *   B = updates per journal transaction (one commit block per transaction)      = 32
 *
 *   ext4 data=journal   : 2 * (1 + M/c) + 1/B   (data and metadata written twice)
 *   ext4 data=ordered   : 1 + 2*M/c + 1/B       (data once, metadata journal + checkpoint)
 *   ext4 data=writeback : 1 + 2*M/c + 1/B       (same cost, weaker ordering guarantee)
 *   copy-on-write       : 1 + D/c               (new data block + rewritten tree path, no double write)
 */
#include <stdio.h>

int main(void)
{
    const double M = 2, D = 3, B = 32;
    const double cs[4] = {1, 4, 16, 64};
    printf("Block writes per 4 KiB random update (lower is better)\n");
    printf("%5s | %12s %12s %14s %12s\n", "c", "data=journal", "data=ordered", "data=writeback", "CoW");
    for (int i = 0; i < 4; i++) {
        double c = cs[i];
        printf("%5.0f | %12.2f %12.2f %14.2f %12.2f\n", c,
               2 * (1 + M / c) + 1 / B, 1 + 2 * M / c + 1 / B, 1 + 2 * M / c + 1 / B, 1 + D / c);
    }
    printf("\nExtra space (CoW): checksums 4 B per 4 KiB block = %.3f%%; each snapshot keeps only\n"
           "blocks changed since it was taken (O(changed blocks)), old blocks are freed on delete.\n",
           4.0 / 4096 * 100);
    printf("Recovery after power failure: ext4 replays the journal, time O(journal size, e.g. 128 MiB-1 GiB);\n"
           "CoW mounts the last committed root, O(1) (plus an optional intent-log replay in ZFS).\n");
    return 0;
}
