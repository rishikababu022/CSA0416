#!/bin/sh
# Compiles and runs every program, saves the output beside the source and checks the
# two "Complete the Code" programs against their expected outputs.
CFLAGS="-std=c11 -Wall -Wextra"
T=${TMPDIR:-/tmp}
gcc --version | head -n 1
for f in Task02_alloc Task02_fragmentation_demo Task02_calc Task04_sstf Task04_queue Task04_starvation \
         Task01_search_cost Task03_journal_vs_cow; do
    gcc $CFLAGS -o $T/$f $f.c && $T/$f > ${f}_output.txt && echo "ran $f"
done
for f in Task05_model Task04_sim Task06_sim Task06_calc; do
    gcc $CFLAGS -o $T/$f $f.c -lm && $T/$f > ${f}_output.txt && echo "ran $f"
done
sh Task01_create_layout.sh   > Task01_create_layout_output.txt 2>&1
sh Task03_inspect.sh         > Task03_inspect_output.txt 2>&1
sh Task03_mkfs_commands.sh   > Task03_mkfs_commands_output.txt 2>&1
sh Task04_scheduler_check.sh > Task04_scheduler_check_output.txt 2>&1
sh Task06_run_fio.sh         > Task06_run_fio_output.txt 2>&1

diff Task02_expected.txt Task02_alloc_output.txt && echo "Task 2 output matches the expected output"
diff -b Task04_expected.txt Task04_sstf_output.txt && echo "Task 4 output matches the expected output (ignoring the column padding of %-6s)"
