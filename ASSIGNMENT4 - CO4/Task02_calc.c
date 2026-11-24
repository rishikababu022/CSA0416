/* Task 2.3 / 2.4 calculations: disk reads, inode maximum file size, free-space structures. */
#include <stdio.h>

int main(void)
{
    const unsigned long long BS = 4096ULL;                 /* block size */
    const unsigned long long FILE_BYTES = 4ULL << 30;      /* 4 GiB file */
    const unsigned long long nblocks = FILE_BYTES / BS;    /* 2^20 */
    const unsigned long long ptrs = BS / 4;                /* 4-byte addresses -> 1024 per block */
    const unsigned long long last = nblocks - 1;           /* index of the last block */

    printf("== Task 2.3: 4 GiB file, 4 KiB blocks ==\n");
    printf("blocks in file                         : %llu\n", nblocks);
    printf("contiguous  : 1 disk read (start + %llu computed arithmetically)\n", last);
    printf("linked      : %llu disk reads (follow %llu pointers, one per block)\n", nblocks, last);
    printf("FAT in RAM  : %llu memory look-ups + 1 disk read for the data block\n", last);

    unsigned long long d = 12, s = ptrs, dd = ptrs * ptrs, t = ptrs * ptrs * ptrs;
    const char *where;
    int reads;
    if (last < d) { where = "direct"; reads = 1; }
    else if (last < d + s) { where = "single indirect"; reads = 2; }
    else if (last < d + s + dd) { where = "double indirect"; reads = 3; }
    else { where = "triple indirect"; reads = 4; }
    printf("indexed (inode): last block is in the %s area -> %d disk reads "
           "(+1 if the inode itself is not cached)\n", where, reads);

    unsigned long long total_blocks = d + s + dd + t;
    unsigned long long bytes = total_blocks * BS;
    printf("\nInode with 12 direct + 1 single + 1 double + 1 triple, 4-byte pointers:\n");
    printf("  direct = %llu, single = %llu, double = %llu, triple = %llu blocks\n", d, s, dd, t);
    printf("  total blocks = %llu\n", total_blocks);
    printf("  max file size = %llu bytes = %.4f TiB\n", bytes, (double)bytes / (1ULL << 40));

    printf("\n== Task 2.4: 4 TB disk, 4 KiB blocks ==\n");
    unsigned long long disk_bytes = 1ULL << 42;            /* 4 TiB (binary) */
    unsigned long long dblocks = disk_bytes / BS;          /* 2^30 */
    printf("blocks on disk           : %llu (2^30)\n", dblocks);
    printf("bit vector               : %llu bits = %llu MiB\n", dblocks, dblocks / 8 / (1ULL << 20));
    printf("FAT (4 B per block)      : %llu GiB of FAT (must be in RAM for speed)\n",
           dblocks * 4 / (1ULL << 30));
    printf("linked free list         : 0 extra bytes (pointer lives inside each free block); "
           "finding k free blocks = O(k) disk reads\n");
    printf("grouping (%llu addr/block): up to %llu group blocks, stored inside free blocks\n",
           ptrs - 1, dblocks / (ptrs - 1));
    printf("counting (start,count)   : 8 bytes per free run; size depends on fragmentation "
           "(very small when free space is contiguous)\n");
    return 0;
}
