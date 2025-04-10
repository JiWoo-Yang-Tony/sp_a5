// FILE NAME    : shared.h
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, Jon Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-4-8
// DESCRIPTION  : Shared header for all processes in HISTO-SYSTEM

#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

// Constants
#define BUF_SIZE 256           // Circular buffer size
#define ALPHABET_COUNT 20      // Number of letters A to T
#define SHM_KEY 0x1234         // Shared memory key
#define SEM_KEY 0x5678         // Semaphore key
#define HISTOGRAM_INTERVAL 10  // Histogram display interval (seconds)
#define READ_AMOUNT 30         // Number of letters to read every 2 seconds

// Shared memory structure
typedef struct {
    char buffer[BUF_SIZE];     // Circular buffer of letters
    int readIndex;             // Index for DC to read
    int writeIndex;            // Index for DP-1/DP-2 to write
} SharedMemory;

// Semaphore operations (helper functions)
void sem_wait(int semid);      // Wait (P operation)
void sem_signal(int semid);    // Signal (V operation)

// Signal handling flag (for all processes)
extern volatile sig_atomic_t shutdown_flag;

#endif
