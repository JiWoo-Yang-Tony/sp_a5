// FILE NAME    : signal_utils.h
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, John Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-04-11
// DESCRIPTION  : Header file for signal handling functions
#ifndef SIGNAL_UTILS_H
#define SIGNAL_UTILS_H

#include <signal.h>

void sigalarm_handler(int sig);
void sigint_handler(int sig);

#endif
