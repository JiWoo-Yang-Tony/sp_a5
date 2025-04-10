// FILE NAME    : shared.h
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, John Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-4-8
// DESCRIPTION  : 

#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>

// Constants
#define BUF_SIZE 256           // Circular buffer size
#define ALPHABET_COUNT 20      // Number of letters A to T
#define SHM_KEY 0x1234         // Shared memory key
#define SEM_KEY 0x5678         // Semaphore key

// Shared memory structure
typedef struct {
    char buffer[BUF_SIZE];     // Circular buffer of letters
    int readIndex;             // Index for DC to read
    int writeIndex;            // Index for DP-1/DP-2 to write
} SharedMemory;

// Semaphore operations (helper functions)
void sem_wait(int semid);      // Wait (P operation)
void sem_signal(int semid);    // Signal (V operation)

#endif
