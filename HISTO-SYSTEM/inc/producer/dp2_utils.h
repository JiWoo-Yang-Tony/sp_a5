// FILE NAME    : dp2_utils.h
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, John Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-04-11
// DESCRIPTION  : Holds the utility functions for DP-2 process

#ifndef DP2_UTILS_H
#define DP2_UTILS_H

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
#include "dp2.h"

extern SharedMemory *global_shmPtr;

void write_one_letter(SharedMemory *shmPtr, int semID);
void handle_sigint(int sig);

#endif // DP2_UTILS_H