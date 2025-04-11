// FILE NAME    : process_utils.h
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, John Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-04-11
// DESCRIPTION  : Header file for process utility functions
//
#ifndef PROCESS_UTILS_H
#define PROCESS_UTILS_H

void print_histogram(int *histogram);
void run_consumer_loop(int *histogram, int shmID, int *readIndex, int *secondsElapsed);

#endif
