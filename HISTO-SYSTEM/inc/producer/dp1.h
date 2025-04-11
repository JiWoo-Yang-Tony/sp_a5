// FILE NAME    : dp1.h
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, John Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-4-10
// DESCRIPTION  : Function declarations for DP-1 process

#ifndef DP1_H
#define DP1_H

#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

#include "../../inc/shared.h"
#include "dp1_utils.h"

extern SharedMemory* global_shmPtr;

void initialize_shared_memory(int *shmID, SharedMemory **shmPtr);

int initialize_semaphore();

void write_random_letters(SharedMemory *shmPtr, int semID);

#endif
