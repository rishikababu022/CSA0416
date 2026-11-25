/* Task 2 extra: the same disk as Task02_alloc.c, but E needs 14 blocks.
 * Free space = 21 blocks (8 + 13), yet the largest hole is 13, so first-fit
 * contiguous allocation fails (external fragmentation) while indexed
 * allocation succeeds (13 data blocks + 1 index block is not needed; 14 data
 * + 1 index = 15 <= 21). */
#include <stdio.h>
#include <string.h>
#define BLOCKS 32
static char disk[BLOCKS];

static int alloc_contiguous(char file, int size)
{
    int run = 0;
    for (int b = 0; b < BLOCKS; b++) {
        if (disk[b] == '.') {
            if (++run == size) {
                for (int i = b - size + 1; i <= b; i++) disk[i] = file;
                return b - size + 1;
            }
        } else run = 0;
    }
    return -1;
}
static int alloc_indexed(char file, int size, int index[])
{
    int got = 0, idx = -1;
    for (int b = 0; b < BLOCKS && got < size; b++) {
        if (disk[b] != '.') continue;
        if (idx < 0) idx = b; else index[got++] = b;
    }
    if (idx < 0 || got < size) return -1;
    disk[idx] = (char)(file + 32);
    for (int i = 0; i < size; i++) disk[index[i]] = file;
    return idx;
}
static void release(char f) { for (int b = 0; b < BLOCKS; b++) if (disk[b] == f) disk[b] = '.'; }

int main(void)
{
    memset(disk, '.', BLOCKS);
    alloc_contiguous('A', 6); alloc_contiguous('B', 8);
    alloc_contiguous('C', 5); alloc_contiguous('D', 7);
    release('B'); release('D');
    int free_blocks = 0;
    for (int b = 0; b < BLOCKS; b++) free_blocks += disk[b] == '.';
    printf("%.32s  free blocks = %d\n", disk, free_blocks);
    printf("E (14 blocks) contiguous: start %d  (-1 = failed)\n", alloc_contiguous('E', 14));
    int index[32];
    int ib = alloc_indexed('E', 14, index);
    printf("E (14 blocks) indexed: index block %d\n%.32s\n", ib, disk);
    return 0;
}
