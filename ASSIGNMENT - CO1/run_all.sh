#!/bin/bash
# Compile and run every program; saves outputs as Taskxx_output.txt (run from this directory)
set -u
CF="-std=c11 -Wall -Wextra -pthread"
gcc $CF Task06_copy_file.c -o t6_copy
gcc $CF Task06_bench_copy.c -o t6_bench
gcc $CF Task06_mmap_copy.c -o t6_mmap
gcc $CF Task05_fopen_vs_read.c -o t5_cmp
gcc $CF Task05_hello_syscall.c -o t5_hello
gcc $CF Task07_shell.c -o t7_shell
gcc $CF Task07_shell_noflush.c -o t7_noflush
gcc $CF Task07_posix_spawn.c -o t7_spawn
gcc $CF Task07_fork_exec_timing.c -o t7_forkexec
gcc $CF Task08_polling_vs_interrupt.c -o t8_poll
gcc $CF Task10_fork_vs_threads.c -o t10_ft

./t6_copy > Task06_output.txt
./t6_bench 1 >> Task06_output.txt; ./t6_bench 16 >> Task06_output.txt
./t7_shell > Task07_output.txt
# (terminal run: ./t7_noflush with no redirection - line buffered, no duplicate)
./t7_noflush > Task07_noflush_redirected.txt        # redirected: "job 2" appears twice
./t7_forkexec 1000 > Task07_timing_output.txt
./t7_spawn 1000 >> Task07_timing_output.txt
./t8_poll > Task08_output.txt
python3 Task09_dma_vs_interrupt.py > Task09_output.txt
./t10_ft > Task10_output.txt
( time ./t6_bench 16 ) 2> Task10_time_output.txt >/dev/null

if command -v strace >/dev/null; then
  strace -c ./t6_copy 2> Task06_strace_c.txt >/dev/null
  strace -o Task05_strace_hello.txt ./t5_hello >/dev/null
  strace -c ./t5_cmp lib 2> Task05_strace_lib.txt
  strace -c ./t5_cmp sys 512 2> Task05_strace_sys512.txt
  strace -c ./t5_cmp sys 4096 2> Task05_strace_sys4096.txt
  strace -f -e trace=clone,fork,vfork,execve,wait4 ./t7_shell 2> Task07_strace.txt >/dev/null
else
  echo "strace not installed: sudo apt install strace, then re-run" | tee Task05_strace_MISSING.txt
fi
rm -f t6_* t5_* t7_* t8_* t10_* bench_*.dat
