#!/bin/bash
CFLAGS="-std=c11 -Wall -Wextra -D_GNU_SOURCE"
echo "=== Q2 Complete the Code: page size 1024 ==="
gcc $CFLAGS -o Q2_paging Q2_paging.c && ./Q2_paging
echo; echo "Expected:"; cat <<'X'
0 -> page 0 offset 0 : physical 5120
1500 -> page 1 offset 476 : physical 2524
3100 -> page 3 offset 28 : physical 7196
6200 -> page 6 offset 56 : physical 3128
9000 -> page 8 offset 808 : invalid address
2049 -> page 2 offset 1 : page fault
X
echo; echo "=== PAGE_SIZE changed to 2048 (hand calculation in README.md) ==="
sed 's/#define PAGE_SIZE 1024/#define PAGE_SIZE 2048/' Q2_paging.c > Q2_paging_2048.c
gcc $CFLAGS -o Q2_paging_2048 Q2_paging_2048.c && ./Q2_paging_2048
echo; echo "=== Q2 calculations ==="
gcc $CFLAGS -o Q2_calc Q2_calc.c -lm && ./Q2_calc
echo; echo "=== getconf PAGESIZE ==="; getconf PAGESIZE
echo; echo "=== page faults of a program (/usr/bin/time -v) ==="
if [ -x /usr/bin/time ]; then /usr/bin/time -v ./Q2_calc 2>&1 >/dev/null | grep -E "page faults|Maximum resident"; else echo "/usr/bin/time not installed here (apt install time); use: /usr/bin/time -v ./Q2_calc"; fi
echo; echo "=== Alternative: inverted page table ==="
gcc $CFLAGS -o Q2_inverted Q2_inverted.c -lm && ./Q2_inverted
