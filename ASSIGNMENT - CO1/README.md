# CSA04 Operating Systems - Assignment 1 (CO1): Smart Hospital OS Design

- Name: Rishika B
- Register number: <fill in>
- Compiler: gcc 13.3.0 (Ubuntu 13.3.0-6ubuntu2~24.04.1)
- OS: Ubuntu 24.04 LTS (Linux)
- Compile command: `gcc -std=c11 -Wall -Wextra -pthread <file>.c -o <name>`
- Repository name to create: `CSA04-<RegisterNumber>-Assignment1` (this zip is flat, so files are prefixed with the task number; put each task's files in its own folder `TaskNN/` in the repo and commit one task at a time)

## Files by task
| Task | Files | What it does |
|---|---|---|
| 5 | Task05_hello_syscall.c, Task05_fopen_vs_read.c | strace evidence; fopen/fread/fwrite vs read/write call counts |
| 6 | Task06_copy_file.c (TODO 1 done), Task06_bench_copy.c, Task06_mmap_copy.c, Task06_output.txt | buffer size vs read() calls and time; mmap alternative |
| 7 | Task07_shell.c (TODO 1 done), Task07_shell_noflush.c, Task07_posix_spawn.c, Task07_fork_exec_timing.c, Task07_*.txt | fork/exec/waitpid shell; flush experiment; posix_spawn vs fork+exec |
| 8 | Task08_polling_vs_interrupt.c, Task08_output.txt | polling interval vs signal (interrupt) latency and CPU use |
| 9 | Task09_dma_vs_interrupt.py, Task09_output.txt | interrupt count / CPU time: per-word interrupts vs DMA for 500 MB |
| 10 | Task10_fork_vs_threads.c, Task10_output.txt, Task10_time_output.txt | 1,000 fork vs 1,000 pthread_create; `time` user/sys split |
| all | run_all.sh | compiles and runs everything, regenerates outputs |

Tasks 1-4, 11 and 12 are written analysis (no code).

## How to run
`bash run_all.sh` (needs gcc, python3; install `strace` first: `sudo apt install strace`). Without strace the strace output files are skipped; run these by hand and keep screenshots:
- `strace -c ./t6_copy` (compare read counts with the program output; note the extra read for EOF and the 4096-byte case)
- `strace ./t5_hello`, `strace -c ./t5_cmp lib`, `strace -c ./t5_cmp sys 512`
- `strace -f -e trace=clone,execve,wait4 ./t7_shell`
(compile first, and do not let run_all.sh delete the binaries, or compile manually.)

## Results (measured on the machine above; your numbers will differ)
- Task 6: output matches expected: 10001 / 21 / 4 read() calls. 16 MB copy: 16 B buffer 0.56 s, 512 B 0.042 s, 4096 B 0.025 s, 1 MB 0.013 s.
- Task 7: output matches expected. With the second fflush removed, exit(3) and stdout redirected to a file, the unflushed buffer is copied into the child by fork() and flushed again by exit(), so earlier lines appear twice (`_exit` skips the flush). Terminal output is line-buffered so no duplicates appear.
- Task 7 timing: 1000 x fork+exec ~0.85 s, posix_spawn ~0.71 s.
- Task 8: polling every 1 ms costs ~28x the CPU of the interrupt version; delay is bounded by the polling interval, interrupt is immediate. (Delays for short intervals are timer noise.)
- Task 9: 500 MB = 131,072,000 word interrupts (~262 s CPU at 2 us each, assumed) vs 8,000 DMA interrupts.
- Task 10: 1000 fork+wait ~0.62 s vs 1000 thread create+join ~0.02 s (~30x).

Note: the 2 us interrupt cost and 64 KB DMA block in Task 9 are assumptions; state them in your report.
