// FILE NAME    : dp1_utils.h
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, John Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-04-11
// DESCRIPTION  : Header file for DP-1 utility functions including signal handling,
//                shared memory, semaphore, and circular buffer writing.

#ifndef DP1_UTILS_H
#define DP1_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "../../inc/shared.h"
#include "dp1.h"

extern SharedMemory *global_shmPtr;

void handle_sigint(int sig);
void initialize_shared_memory(int *shmID, SharedMemory **shmPtr);
int initialize_semaphore();
void write_random_letters(SharedMemory *shmPtr, int semID);

#endif // DP1_UTILS_H
