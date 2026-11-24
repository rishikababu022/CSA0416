/* Task 5 alternative concept: one large NFS server versus a replicated distributed file
 * system (HDFS/Ceph-like, 10 storage nodes, 3 replicas).
 *
 * Assumptions (change them and re-run):
 *   every server: 10 Gbit/s network = 1250 MB/s read, 800 MB/s write
 *   every user reads, then writes, a 100 MB file at the same time
 *   DFS: reads are spread over all 10 nodes; every write goes to 3 replicas
 *   each server is up 99% of the time; replica failures are independent
 */
#include <stdio.h>
#include <math.h>

int main(void)
{
    const int users[5] = {10, 100, 1000, 5000, 10000};
    const double file_mb = 100, rd = 1250, wr = 800;
    const int nodes = 10, replicas = 3;

    printf("%8s | %14s %14s | %14s %14s\n", "users", "NFS read (s)", "NFS write (s)", "DFS read (s)", "DFS write (s)");
    for (int i = 0; i < 5; i++) {
        double u = users[i];
        double nfs_r = u * file_mb / rd, nfs_w = u * file_mb / wr;
        double dfs_r = u * file_mb / (rd * nodes);
        double dfs_w = u * file_mb * replicas / (wr * nodes);
        printf("%8d | %14.1f %14.1f | %14.1f %14.1f\n", users[i], nfs_r, nfs_w, dfs_r, dfs_w);
    }
    printf("\nStorage for 100 TB of user data: NFS = 100 TB (1 copy) ; DFS = %d TB (%d replicas)\n",
           100 * replicas, replicas);
    double a = 0.99;
    double dfs_av = 1.0 - pow(1.0 - a, replicas);
    printf("Availability of a file: NFS = %.4f%% (%.1f h/year down) ; DFS = %.4f%% (%.4f h/year down)\n",
           a * 100, (1 - a) * 8760, dfs_av * 100, (1 - dfs_av) * 8760);
    printf("Complexity: NFS read/write of n users O(n) on one server; DFS O(n / nodes) per node, "
           "write cost O(replicas) per block, metadata via a namenode/MDS.\n");
    return 0;
}
