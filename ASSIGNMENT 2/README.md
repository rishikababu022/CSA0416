# Operating Systems – Assignment 2 (CO2)

**Student:** Rishika.B  
**Register number:** 1924260081

This ZIP contains standalone source files for Q1–Q6. All files are placed in the ZIP root; there are no task folders.

## Environment
- Language: C (C11)
- Suggested compiler: GCC
- Platform: Linux
- Compile commands are listed below. Add `-pthread` for thread/semaphore programs.

## Files and execution

| File | Task | Compile and run |
|---|---|---|
| `Q1_processes.c` | Processes, `fork()`, process IDs and `/proc` | `gcc -std=c11 -Wall -Wextra Q1_processes.c -o q1 && ./q1` |
| `Q2_round_robin.c` | Round Robin CPU scheduling (quantum 3) | `gcc -std=c11 -Wall -Wextra Q2_round_robin.c -o q2 && ./q2` |
| `Q3_threads.c` | POSIX threads and camera-frame workload | `gcc -std=c11 -Wall -Wextra -pthread Q3_threads.c -o q3 && ./q3` |
| `Q4_pipe.c` | Parent-child communication using a pipe | `gcc -std=c11 -Wall -Wextra Q4_pipe.c -o q4 && ./q4` |
| `Q5_producer_consumer.c` | Bounded buffer using semaphores and mutex | `gcc -std=c11 -Wall -Wextra -pthread Q5_producer_consumer.c -o q5 && ./q5` |
| `Q6_bankers.c` | Banker’s safety and resource-request algorithms | `gcc -std=c11 -Wall -Wextra Q6_bankers.c -o q6 && ./q6` |

## Notes
- Run each program on Linux and capture your own terminal output/screenshots as assignment evidence.
- For Q1, while the program is running, use another terminal to inspect processes with `ps` and `/proc/<PID>/status`.
- Q2 uses the assignment's four processes, arrival times, burst times and quantum of 3.
- Q3 reports elapsed time for sequential processing and thread-pool sizes 4, 8 and 16. Timing varies by machine.
- Q4 demonstrates EOF: the parent closes the write end after sending all readings.
- Q5 uses one producer and one consumer, matching the supplied starter program. The expected sum is 210.
- Q6 uses the assignment's supplied allocation, maximum and available-resource values and tests the three specified requests.

These are implementation files, not a completed written report. Add your own measured timings, screenshots, diagrams and evaluation to your submission. Create the required GitHub repository and commit work progressively as instructed in the assignment.
