# Codexion: Concurrency and Resource Allocation Simulator

## Overview

**Codexion** is a C-based simulation project designed to explore the challenges of multi-threaded programming. The core problem revolves around managing shared resources (USB dongles) among multiple threads (coders) while preventing fatal concurrency issues like Deadlocks, Data Races, and thread Starvation.

## Compilation & Execution

To compile the project, a simple `Makefile` is provided:
```bash
make
```

Once compiled, run the simulation by passing the mandatory arguments:
```bash
./codexion <num_coders> <burnout> <compile> <debug> <refactor> <req_compiles> <cooldown> <scheduler>
```

### Argument Breakdown:
* **`num_coders`**: How many coders (and dongles) are at the table.
* **`burnout`**: Time limit (ms) a coder has to start their next compilation before failing.
* **`compile`**: Time (ms) required to hold two dongles and compile.
* **`debug`**: Time (ms) spent debugging (no dongles needed).
* **`refactor`**: Time (ms) spent refactoring (no dongles needed).
* **`req_compiles`**: Target number of compilations to stop the simulation.
* **`cooldown`**: Time (ms) a dongle must rest on the table before being acquired again.
* **`scheduler`**: Determines the queue policy (`fifo` or `edf`).

## Core Architecture

The simulation operates in a circular environment. If there are 4 coders, they are seated in a ring with 4 dongles placed between them. A coder must successfully acquire both the left and right dongle to begin compiling. 

```text
                  (CODER 1)
                  /         \
            [Dongle 3]   [Dongle 0]
               /               \
         (CODER 4)           (CODER 2)
               \               /
            [Dongle 2]   [Dongle 1]
                  \         /
                   (CODER 3)
```

### Thread Lifecycle
1. **Queueing:** Coders enter a waiting room managed by condition variables (`pthread_cond_t`). They are woken up based on the selected scheduler policy (FIFO for first-come-first-serve, or EDF for prioritizing coders closest to burnout).
2. **Acquisition:** The coder attempts to lock the required dongles (`pthread_mutex_lock`).
3. **Hardware Cooldown:** If a dongle was recently used, the coder sleeps for the remaining `cooldown` duration while holding the lock to prevent others from bypassing the restriction.
4. **Action Loop:** The coder updates their timestamp, compiles, releases the dongles, and proceeds to debug and refactor.

### The Supervisor (Monitor)
A detached thread acts as the ultimate authority. It iterates through the array of coders every millisecond, checking if `current_time - last_compile_start` exceeds the `burnout` limit. If a burnout is detected, the monitor halts the simulation immediately. It also tracks if all coders have reached the `req_compiles` goal.

## Preventing Concurrency Disasters

This project explicitly solves several classic synchronization problems:

* **Breaking the Circular Wait (Deadlock Prevention):**
  If every coder grabs their left dongle simultaneously, the simulation freezes permanently. To prevent this, an asymmetric approach is used. The very last coder at the table (`id == num_coders`) is programmed to grab their **right** dongle first. This forces two coders to compete for the exact same first mutex, ensuring that at least one coder gets blocked empty-handed, leaving enough resources on the table for someone else to proceed.

* **Avoiding Busy-Waiting (Starvation Prevention):**
  Instead of using infinite `while` loops that consume 100% of the CPU to check if a turn has arrived, the scheduler leverages POSIX condition variables. Coders safely sleep via `pthread_cond_wait` and are only awakened by `pthread_cond_broadcast` when resources are freed.

* **Data Race Immunity:**
  Every shared variable (such as timestamps, print logs, and death flags) is guarded by a dedicated `pthread_mutex_t`. When the Monitor reads a coder's timestamp, it briefly locks the coder's personal `time_mutex` to ensure it doesn't read a half-written value.

## Study Materials

For those looking to dive deeper into systems programming and synchronization, these resources were instrumental during development:
* **[OSTEP: Concurrency](https://pages.cs.wisc.edu/~remzi/OSTEP/#concurrency)** - Chapters 28 (Locks) and 30 (Condition Variables) are essential reading.
* **[CodeVault: Pthreads in C](https://www.youtube.com/playlist?list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2)** - Excellent visual explanations of mutexes and threads.
* **[Lawrence Livermore National Laboratory: POSIX Threads](https://hpc-tutorials.llnl.gov/posix/)** - The definitive guide on pthreads.