// FILE NAME    : dc.h
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, Jon Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-4-10
// DESCRIPTION  : Header file for DC process

#ifndef DC_H
#define DC_H

#include <signal.h>
#include "../../inc/shared.h"
#include "process_utils.h"
#include "signal_utils.h"

extern volatile sig_atomic_t readFlag;
extern volatile sig_atomic_t shutdownFlag;
extern pid_t dp1PID;
extern pid_t dp2PID;
extern int semID;
extern SharedMemory *shmPtr;

#endif