/* Task 6 Stage 4: capacity, throughput and disk reads to start playback at any minute. */
#include <stdio.h>
#include <math.h>

static int reads_indirect(unsigned long long blk)  /* inode cached; data block read included */
{
    if (blk < 12) return 1;
    if (blk < 12 + 1024) return 2;
    if (blk < 12 + 1024 + 1024ULL * 1024) return 3;
    return 4;
}

int main(void)
{
    const int videos = 5000;
    const double avg_gb = (0.3 + 2.0) / 2.0;
    const double raw_tb = videos * avg_gb / 1000.0;
    const int replicas = 3;
    printf("== Capacity ==\n");
    printf("videos %d x average %.2f GB = %.2f TB raw\n", videos, avg_gb, raw_tb);
    printf("x%d replicas (2 on-premise + 1 cloud copy) = %.2f TB; with 20%% free space on-premise: %.2f TB\n",
           replicas, raw_tb * replicas, raw_tb * 2 / 0.8);

    const double bitrate_mbps = 4.0, mbyte_s = bitrate_mbps / 8.0;
    const int students = 200000;
    const double concurrent = students * 0.10;
    printf("\n== Throughput ==\n");
    printf("bit-rate %.0f Mbit/s = %.2f MB/s per stream\n", bitrate_mbps, mbyte_s);
    printf("peak concurrent viewers (10%% of %d) = %.0f\n", students, concurrent);
    printf("aggregate = %.0f MB/s = %.1f GB/s = %.0f Gbit/s\n",
           concurrent * mbyte_s, concurrent * mbyte_s / 1000, concurrent * bitrate_mbps / 1000);

    const double chunk_mb = 8.0, hdd_bw = 150.0, hdd_pos = 4.2 + 9.0;
    double t_read = hdd_pos + chunk_mb / hdd_bw * 1000.0;
    double period_ms = chunk_mb / mbyte_s * 1000.0;
    double per_hdd = floor(period_ms / t_read);
    printf("HDD: one %.0f MB chunk takes %.1f ms; a stream needs one every %.0f ms -> %.0f streams/disk "
           "(%.0f at 70%% utilisation)\n", chunk_mb, t_read, period_ms, per_hdd, floor(per_hdd * 0.7));
    printf("without cache: %.0f HDDs needed for %.0f streams\n",
           ceil(concurrent / floor(per_hdd * 0.7)), concurrent);
    double hit = 0.85;
    printf("with an assumed %.0f%% served from SSD/RAM/CDN: %.0f HDD streams -> %.0f HDDs;\n",
           hit * 100, concurrent * (1 - hit), ceil(concurrent * (1 - hit) / floor(per_hdd * 0.7)));
    printf("  cached streams %.0f x %.2f MB/s = %.1f GB/s -> %.0f NVMe SSDs at 3 GB/s (N+1 extra)\n",
           concurrent * hit, mbyte_s, concurrent * hit * mbyte_s / 1000,
           ceil(concurrent * hit * mbyte_s / 3000.0));
    printf("network: %.0f Gbit/s / 50 Gbit/s per server (2 x 25 GbE) = %.0f servers (+1 spare)\n",
           concurrent * bitrate_mbps / 1000, ceil(concurrent * bitrate_mbps / 1000 / 50));

    printf("\n== Disk reads to start playing at minute m of a 2 GB video (4 KiB blocks) ==\n");
    printf("%8s %14s %26s %24s\n", "minute", "block number", "classic indirect inode", "extent tree (ext4/XFS)");
    int minutes[4] = {0, 1, 10, 60};
    for (int i = 0; i < 4; i++) {
        unsigned long long blk = (unsigned long long)minutes[i] * 60ULL * 500000ULL / 4096ULL;
        printf("%8d %14llu %20d reads %20s\n", minutes[i], blk, reads_indirect(blk),
               "1 (cached) to 2 reads");
    }
    printf("(an extent tree needs at most one extent-index read for a 2 GB file of 16 extents of 128 MiB,\n"
           " then one data read; the 4 MB start-up buffer is then 1 sequential read of 1024 blocks)\n");
    return 0;
}
