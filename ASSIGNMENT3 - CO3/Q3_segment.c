/* Q3: segment-table translation (base, limit), limit violation, and segmentation with paging (x86 style). */
#include <stdio.h>
typedef struct { unsigned base, limit; const char *name; } Seg;
Seg st[5] = {{1400, 1000, "code"}, {6300, 400, "stack"}, {4300, 400, "data"}, {3200, 1100, "heap"}, {4700, 1000, "shared lib"}};

static void translate(unsigned s, unsigned off)
{
    if (s >= 5) { printf("(%u,%4u) -> TRAP: invalid segment number\n", s, off); return; }
    if (off >= st[s].limit) { printf("(%u,%4u) -> TRAP: offset %u >= limit %u of segment %u (%s) -> addressing error, OS terminates process\n", s, off, off, st[s].limit, s, st[s].name); return; }
    printf("(%u,%4u) -> physical %u + %u = %u  [%s]\n", s, off, st[s].base, off, st[s].base + off, st[s].name);
}

/* x86 (32-bit) segmentation with paging: logical -> (segment base + offset) = linear -> 10|10|12 split */
static void seg_paging(unsigned seg_base, unsigned off)
{
    unsigned lin = seg_base + off, dir = lin >> 22, tbl = (lin >> 12) & 0x3FF, pgoff = lin & 0xFFF;
    printf("logical (segment base 0x%08X, offset 0x%X): linear = 0x%08X -> directory index %u, page-table index %u, page offset 0x%X\n"
           "   physical = frame(PDE[%u] -> PT[%u]) * 4096 + 0x%X   (two extra memory accesses unless the TLB hits)\n",
           seg_base, off, lin, dir, tbl, pgoff, dir, tbl, pgoff);
}

int main(void)
{
    printf("Segment table (base, limit):\n");
    for (int i = 0; i < 5; i++) printf("  seg %d %-10s base %5u limit %4u\n", i, st[i].name, st[i].base, st[i].limit);
    printf("\n8 addresses translated (verify by hand: base + offset if offset < limit):\n");
    unsigned t[8][2] = {{0, 53}, {1, 852}, {2, 0}, {3, 1099}, {3, 1100}, {4, 999}, {2, 399}, {1, 400}};
    for (int i = 0; i < 8; i++) translate(t[i][0], t[i][1]);
    printf("  extra: "); translate(7, 5);
    printf("\nSegmentation with paging:\n");
    seg_paging(0x08048000, 0x1234);
    seg_paging(0xBF000000, 0x0FFC);
    return 0;
}
