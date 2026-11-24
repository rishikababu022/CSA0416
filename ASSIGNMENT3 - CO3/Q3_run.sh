#!/bin/bash
CFLAGS="-std=c11 -Wall -Wextra -D_GNU_SOURCE"
for f in Q3_segment Q3_maps Q3_frag Q3_segfault; do gcc $CFLAGS -o $f $f.c || exit 1; done
echo "=== Q3 segment-table translation ==="; ./Q3_segment
echo; echo "=== Segments of a running Linux program (/proc/<pid>/maps) ==="; ./Q3_maps
echo; echo "=== Segmentation faults ==="
for m in null write stack; do echo "--- $m ---"; ( ./Q3_segfault $m; echo "exit status $? (139 = 128 + SIGSEGV(11))" ) 2>&1; done
echo "--- with a SIGSEGV handler (shows the fault address given by the kernel) ---"
( ./Q3_segfault handler; echo "exit status $?" ) 2>&1
echo; echo "=== Fragmentation and table sizes ==="; ./Q3_frag
