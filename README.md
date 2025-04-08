# HISTO-SYSTEM: Inter-Process Communication Histogram System

The goal is to implement a Linux-based inter-process communication system in ANSI C using **shared memory**, **semaphores**, and **signals**. The system consists of three separate executables:

- `DP-1`: Data Producer 1
- `DP-2`: Data Producer 2
- `DC`: Data Consumer

---

## Project Structure

```text
HISTO-SYSTEM/
├── bin/
│   ├── DP-1
│   ├── DP-2
│   └── DC
├── src/
│   ├── dp1.c
│   ├── dp2.c
│   └── dc.c
├── inc/
│   └── shared.h
├── Makefile
└── README.md
```

---

## System Overview

- **DP-1** creates shared memory and a semaphore.
- **DP-2** is launched by DP-1 and attaches to shared memory.
- **DC** is launched by DP-2, and it reads from the buffer and prints a histogram every 10 seconds.
- Shared memory is structured as a **circular buffer** of 256 characters (`A` to `T`) with read/write indices.
- All access to the buffer is synchronized via **semaphores**.
- All processes handle **SIGINT** signals for safe termination and cleanup.

---

## Milestones

### Milestone 0: Project Setup ✅
- Create the directory structure.
- Write shared.h for shared constants and data structures.
- Implement Makefile to build all three programs.

### Milestone 1: Shared Memory & Semaphore Initialization (DP-1) ✅
- Allocate and initialize shared memory with read/write indices.
- Create and initialize the semaphore.

### Milestone 2: Launch DP-2 from DP-1 ✅
- Use fork() and exec() to start DP-2 from DP-1.
- DP-2 receives shmID and attaches to shared memory.

### Milestone 3: Writing to Buffer (DP-1 & DP-2) ✅
- DP-1: Writes 20 random letters every 2 seconds.
- DP-2: Writes 1 letter every 1/20 of a second.
- Synchronize writes with the semaphore.
- Handle circular buffer wrap-around and overrun protection.

### Milestone 4: Launch DC from DP-2
- DP-2 starts DC with required arguments (shmID, PIDs).
- DC attaches to shared memory.

### Milestone 5: Histogram Reading & Display (DC)
- DC reads letters from the buffer every 2 seconds.
- Tracks frequency of A–T and displays histogram every 10 seconds.
- Histogram units:
  - `-` for 1s
  - `+` for 10s
  - `*` for 100s

### Milestone 6: SIGINT Handling and Cleanup
- All processes handle SIGINT.
- DC sends SIGINT to both producers on termination.
- Final histogram is printed, resources are cleaned, and "Shazam !!" is displayed.

---

## Development Tips

- Use shared.h to define all shared types, keys, and utility functions.
- Each .c file should only include what it needs and rely on `shared.h` for shared structures.
- Use ipcs to verify shared memory and semaphore creation.
- Use kill -2 <PID> to manually test SIGINT on DC.
- Clean up all resources before exit: shared memory, semaphore, child processes.
- Add debug printf()s during development but remove them before final submission.
