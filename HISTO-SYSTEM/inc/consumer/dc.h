// FILE NAME    : dc.h
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, Jon Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-4-10
// DESCRIPTION  : 

#ifndef DC_H
#define DC_H

#include "../../inc/shared.h"

// Function declarations for DC process
void sigalarm_handler(int signal);

void sigint_handler(int signal);

void print_histogram(int histogram[]);

void run_consumer_loop(int *histogram, int shmID, int *readIndex, int *secondsElapsed);;

#endif