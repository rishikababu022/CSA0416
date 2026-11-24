#!/bin/bash
CFLAGS="-std=c11 -Wall -Wextra -D_GNU_SOURCE -O1"
for f in Q4_eat Q4_touch Q4_cow Q4_framesim; do gcc $CFLAGS -o $f $f.c || exit 1; done
echo "=== Q4 EAT, maximum fault rate, frame allocation ==="; ./Q4_eat
echo; echo "=== Sequential vs random touching, growing memory ==="
echo "free memory:"; free -m | head -2
for mb in 32 128 512; do echo "--- $mb MB ---"; ./Q4_touch $mb 2; done
echo "(To exceed RAM: run in a VM or: systemd-run --user --scope -p MemoryMax=512M -p MemorySwapMax=2G ./Q4_touch 1024 )"
echo; echo "=== Copy-on-write with fork() ==="; ./Q4_cow 100
echo; echo "=== Page faults vs frames (reference string) ==="; ./Q4_framesim
python3 plot_all.py
