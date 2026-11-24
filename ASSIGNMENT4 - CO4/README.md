# CSA04 – Operating Systems – Assignment 4 (CO4): Optimising File Storage and Disk Access

- Name: ____________________    Register number: ____________________
- Repository name: CSA04-<RegisterNumber>-Assignment4
- Compiler: gcc 13.3.0 (Ubuntu 13.3.0-6ubuntu2~24.04.1)
- Linux: Ubuntu 24.04.5 LTS (kernel 6.18)
- Compile command: `gcc -std=c11 -Wall -Wextra -o prog file.c` (add `-lm` for programs that use math.h:
  Task04_sim, Task05_model, Task06_sim, Task06_calc)
- Run everything: `sh run_all.sh` (compiles, runs, writes `*_output.txt`, checks the two Complete-the-Code outputs)
- Make the per-task git history: edit and run `sh git_commits.sh`

All files are in one flat folder; the task number is the file-name prefix.

| Task | Files | Result |
|---|---|---|
| 1 | Task01_create_layout.sh, Task01_search_cost.c (+ `_output.txt`) | Tree `home/<dept>/<year>/<regno>` (0700), shared project 2770 with symlinks, drop box 1733, read-only backup 0500, naming-rule check. Look-up among 12,000 names: flat ~6013 compares, dept/year tree ~159, hashed directory ~24 |
| 2 | Task02_alloc.c (completed TODO 1), Task02_expected.txt, Task02_fragmentation_demo.c, Task02_calc.c | Output matches expected. Last block of 4 GiB file: contiguous 1 read, linked 1,048,576 reads, FAT 1,048,575 RAM look-ups + 1 read, inode 3 reads (double indirect). Max inode file size 1,074,791,436 blocks = 4.0039 TiB. Bit vector for 4 TiB / 4 KiB = 128 MiB; FAT = 4 GiB |
| 3 | Task03_inspect.sh, Task03_mkfs_commands.sh, Task03_journal_vs_cow.c | stat / df -i / ls -i / dumpe2fs / tune2fs evidence; mkfs commands (printed, not executed); write-cost model ext4 journal modes vs copy-on-write |
| 4 | Task04_sstf.c (completed TODO 1), Task04_expected.txt, Task04_queue.c, Task04_sim.c, Task04_starvation.c, Task04_scheduler_check.sh | FCFS 640, SSTF 236, SCAN 236, C-SCAN 382 (183 without the 199 return jump), LOOK 208 (down first) / 299 (up first), C-LOOK 322. With a stream near cylinder 60 every ms, SSTF never serves 183 (starvation); SCAN/LOOK serve it in about 60 ms |
| 5 | Task05_model.c | One NFS server vs 10-node DFS with 3 replicas: read/write time vs users, 1x vs 3x storage, availability 99% vs 99.9999% |
| 6 | Task06_calc.c, Task06_sim.c, Task06_designA_hdd.fio, Task06_designB_ssd.fio, Task06_run_fio.sh | Design A (8 HDD) vs Design B (8 HDD + 1 TB SSD cache). At 100 requests/s: mean 63 ms vs 18 ms, p99 214 ms vs 81 ms. At 150/s Design A saturates (mean 5.6 s), B stays at 20 ms |

## Notes you must know before submitting
1. The assignment says every student must run every task themselves, with screenshots and one git commit per task.
   The outputs here were produced on a cloud Linux machine, not on your computer. Run `sh run_all.sh` on your own
   machine, take screenshots, and replace the `*_output.txt` files with your own results (Task03_inspect and
   Task04_scheduler_check in particular describe the machine they run on).
2. Task 2: with the given disk, E (10 blocks) DOES fit contiguously at block 19, because the free run 19-31 is
   13 blocks long. That is why the expected output shows "start 19". The problem statement's sentence that E cannot be
   stored contiguously is true for a file larger than 13 blocks. Task02_fragmentation_demo.c shows 21 free blocks but a
   largest hole of 13, so a 14-block file fails contiguously (-1) and succeeds with indexed allocation.
   The main program also allocates E twice (contiguous, then indexed) – that is how the expected final disk line arises.
3. Task 4: the completed program prints names padded to 6 characters (`%-6s`, from the given code), so the lines differ
   from Task04_expected.txt only in spaces; run_all.sh compares with `diff -b`.
4. Task04_sim.c and Task06_sim.c are models with the assumptions written at the top of each file (arrival rates, seek and
   transfer times). Heavy load in Task04_sim is unstable for FCFS (wait grows to seconds) – that is the intended result.
   Task06_sim.c is a simulation, not a fio measurement: fio was not installed here. Run Task06_run_fio.sh on your
   machine if you want real device numbers.
5. Task03_journal_vs_cow.c and Task05_model.c are analytical models, not measurements.
6. The report (comparison tables, pseudocode, written answers) is not included; this zip contains code, scripts, configs
   and outputs only.
