#!/bin/bash
CFLAGS="-std=c11 -Wall -Wextra -D_GNU_SOURCE -O2"
gcc $CFLAGS -o Q7_rt_lock Q7_rt_lock.c || exit 1
gcc $CFLAGS -o Q7_sim Q7_sim.c || exit 1
echo "=== Worst-case delay of the time-critical loop (3 runs each) ==="
ulimit -l unlimited 2>/dev/null
for i in 1 2 3; do ./Q7_rt_lock nolock 20000; done
for i in 1 2 3; do ./Q7_rt_lock lock 20000; done
echo; echo "=== Simulation: three tasks (control 64 MB locked, ML 1.5 GB, logger 100 MB) ==="; ./Q7_sim 0
echo; echo "=== Simulation: with a fourth task of 500 MB ==="; ./Q7_sim 500
