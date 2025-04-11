// FILE NAME    : signal_utils.c
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, John Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-04-11
// DESCRIPTION  : Holds the signal handling functions for the DC process
//               1. sigalarm_handler() - Handles SIGALRM signal by setting readFlag
//               2. sigint_handler() - Handles SIGINT signal by setting shutdownFlag and killing DP-1 and DP-2 processes
#include "shared.h"
#include "signal_utils.h"
#include "dc.h"

/*
 * FUNCTION :    signalarm_handler()
 * DESCRIPTION : Handles SIGALRM signal by setting readFlag
 * PARAMETERS :  int signal - signal number
 * RETURNS :     none
 */
void sigalarm_handler(int signal) 
{
    readFlag = 1;
}

/*
 * FUNCTION :    sigint_handler()
 * DESCRIPTION : Handles SIGINT signal by setting shutdownFlag and killing DP-1 and DP-2 processes
 * PARAMETERS :  int signal - signal number
 * RETURNS :     none
 */
void sigint_handler(int signal) 
{
    shutdownFlag = 1;
    kill(dp1PID, SIGINT);
    kill(dp2PID, SIGINT);
}