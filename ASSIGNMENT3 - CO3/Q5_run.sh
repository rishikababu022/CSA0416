#!/bin/bash
CFLAGS="-std=c11 -Wall -Wextra -D_GNU_SOURCE -O2"
gcc $CFLAGS -o Q5_lru_complete Q5_lru_complete.c || exit 1
gcc $CFLAGS -o Q5_experiment Q5_experiment.c || exit 1
echo "=== Q5 Complete the Code (LRU) ==="; ./Q5_lru_complete
echo; echo "Expected:"; cat <<'X'
3 frames: FIFO 15, LRU 12, Optimal 9
4 frames: FIFO 10, LRU 8, Optimal 8
Belady string, FIFO: 3 frames 9 faults, 4 frames 10 faults
X
echo; echo "=== Hand-calculation traces ==="; ./Q5_experiment trace
echo; echo "=== Experiments ==="; ./Q5_experiment
python3 plot_all.py
