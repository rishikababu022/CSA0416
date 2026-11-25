# CSA04 – Operating Systems – Assignment 3 (CO3)

| Field | Value |
|---|---|
| Name | **Rishika B** |
| Register number | **<fill in>** |
| Repository name | `CSA04-<RegisterNumber>-Assignment3` |
| Compiler | gcc 13.3.0 |
| Linux | Ubuntu 24.04 LTS (glibc 2.39) |
| Compile command | `gcc -std=c11 -Wall -Wextra -D_GNU_SOURCE -O2 file.c -o file` (add `-lm` for math, `-ldl` for dlopen) |

> **Before submitting:** run `./run_all.sh` on **your own** machine, replace the `sample_output_Q*.txt` files with your own
> outputs (`output_Q*.txt`), add screenshots, write the register number above, and commit task by task
> (at least one commit per task). The `sample_output_*` files were produced in a Linux sandbox (VM, SSD/HDD flags and
> `fdisk` are not representative); the algorithmic results (Q2, Q3, Q5, Q6, Q7 simulations) are deterministic.

## Layout (flat – the zip has no folders)
To use one folder per task, as the assignment asks, move the files into `Q1/ … Q7/` by their prefix
(`Q1_*` → `Q1/`, …; `plot_all.py`, `run_all.sh`, `Makefile`, `README.md` stay in the root; each `Qn_run.sh` works inside its folder
if `plot_all.py` is copied beside it for Q4 and Q5).

```
mkdir -p Q1 Q2 Q3 Q4 Q5 Q6 Q7; for i in 1 2 3 4 5 6 7; do mv Q${i}_* sample_output_Q${i}.txt Q$i/ 2>/dev/null; done; cp plot_all.py Q4/; cp plot_all.py Q5/
```

## How to run
Requirements: `gcc`, `python3` with `matplotlib` (plots only), standard Linux tools (`lsblk`, `ldd`, `readelf`, `file`).
Run everything: `./run_all.sh`, or one task: `./Q1_run.sh` … `./Q7_run.sh`. Q7 uses `mlockall`: run as root or `ulimit -l unlimited`.

| Task | Files | What it does |
|---|---|---|
| Q1 | `Q1_encrypt.[ch]`, `Q1_encrypt_v2.c`, `Q1_app.c`, `Q1_dlopen_app.c`, `Q1_run.sh` | static vs dynamic build, sizes, `ldd`, `/proc/self/maps`, `dlopen`/`dlsym`, start-up time & memory, library replacement, shared pages (`smaps`), missing library, PIC check |
| Q2 | `Q2_paging.c` (**Complete the Code**), `Q2_calc.c`, `Q2_inverted.c`, `Q2_run.sh` | translation, page-table size, EAT (no TLB / 80 / 95 / 99 %), internal fragmentation (1 KB, 4 KB, 2 MB), page size, page faults, boundary tests, inverted page table |
| Q3 | `Q3_segment.c`, `Q3_maps.c`, `Q3_segfault.c`, `Q3_frag.c`, `Q3_run.sh` | segment table with limit trap, segmentation + paging split, `/proc/<pid>/maps`, segfault (3 causes + handler), fragmentation of 20 segments, table sizes |
| Q4 | `Q4_eat.c`, `Q4_touch.c`, `Q4_cow.c`, `Q4_framesim.c`, `plot_all.py`, `Q4_run.sh` | EAT with faults, max fault rate, frame allocation, sequential/random touching, copy-on-write, faults vs frames (CSV + PNG) |
| Q5 | `Q5_lru_complete.c` (**Complete the Code**), `Q5_experiment.c`, `plot_all.py`, `Q5_run.sh` | FIFO/LRU/Optimal/Clock, Belady, locality vs random 3–10 frames, timing, hand trace |
| Q6 | `Q6_raid.c`, `Q6_run.sh` | RAID capacities, write cost, data-loss probability, `lsblk`/swap/partitions, read speed |
| Q7 | `Q7_rt_lock.c`, `Q7_sim.c`, `Q7_run.sh` | locked vs unlocked worst-case delay, 3-task and 4-task page-replacement simulation |

## Short result note per task (from the sample run – replace with your own numbers)
**Q1.** Static executable ≈ 790 KB vs ≈ 16.7 KB dynamic (+ 15 KB `libencrypt.so`). Replacing `libencrypt.so` by the v2 build changed the
output of `app_dynamic` and `app_dlopen` without recompiling; `app_static` kept the old code. Missing library: `app_dynamic` fails at load time (exit 127),
`app_dlopen` handles it and exits with a message. In `smaps` of two running copies, code pages show `Pss = Rss/2` (shared). Static starts faster per run
(≈0.7 ms vs ≈1.0 ms), but uses no shared pages and needs a rebuild for every security fix.

**Q2.** Output matches the expected output exactly. With `PAGE_SIZE 2048` the results are: 0→10240, 1500→11740, 3100→5148, 6200→14392, 9000→808 (page 4, frame 0), 2049→4097 (page 1, frame 2).
Hand check: 3100 = 1·2048 + 1052 → frame 2 → 2·2048 + 1052 = 5148. Page tables: 4 MB (32-bit) and 512 GB (48-bit, 8 B entries). EAT: 200 ns without TLB; 140 / 125 / 121 ns for 80 / 95 / 99 % hits (TLB 20 ns).
Internal fragmentation ≈ page/2 per process (0.5 KB, 2 KB, ≈1 MB).

**Q3.** 8 translations incl. 3 traps (offset ≥ limit). Segmentation fragments badly (85.7 % external fragmentation after removing 10 of 20 segments, and a 120 KB segment cannot be loaded although 532 KB are free).
Table size: 24 B (segmentation) vs 924 B (segmented paging) vs 4 MB flat page table.

**Q4.** EAT: 8099.9 ns, 180 ns, 108 ns for faults of 1/1000, 1/100000, 1/1000000; max fault rate for 10 % slow-down = 1.25·10⁻⁶. Fork of a 100 MB process ≈ 1 ms;
the child's writes cause 25,600 copy-on-write faults and Private_Dirty rises from 40 kB to ≈102 MB. To measure run time beyond RAM, run `Q4_touch` inside a VM or with a cgroup limit
(command in the source header and in `Q4_run.sh`); the sandbox had 3.9 GB RAM and no swap, so the "beyond RAM" experiment must be repeated on your machine.

**Q5.** Output matches the expected output (15/12/9, 10/8/8, 9/10). Locality workload: LRU ≈ 14 % faults at 10 frames vs FIFO 24 %, Clock in between; random workload: all policies equal (≈ 90–97 % faults).
Running time per 100,000 references is about 0.6 ms (FIFO), 0.9 ms (LRU), 1.1 ms (Optimal), 1.3 ms (Clock) in the sample run; Optimal needs the future and cannot be implemented.

**Q6.** Capacities for 6×4 TB: RAID 0 = 24, RAID 1 (pairs) = 12, RAID 5 = 20, RAID 6 = 16, RAID 10 = 12 TB. Rebuild data-loss probabilities (assumed 3 % AFR, 24 h rebuild):
RAID 1/10 0.0083 %, RAID 5 0.042 %, RAID 6 7·10⁻⁶ %; the unrecoverable-read-error risk during a RAID 5 rebuild is ≈ 80 %.

**Q7.** Worst-case delay of the loop: ≈ 130–190 µs without locking (16,425 page faults in the loop) vs ≈ 28–39 µs with `mlockall` (1 fault). Simulation: with 3 tasks the control task has 0 faults and the ML task only cold faults;
with a 4th 500 MB task the demand (2200 MB) exceeds the usable 1848 MB, the ML fault rate rises to ≈ 4.6 % and the extra task to ≈ 6.9 %, but the locked control task still has 0 faults.
