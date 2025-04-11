#ifndef PROCESS_UTILS_H
#define PROCESS_UTILS_H

void print_histogram(int *histogram);
void run_consumer_loop(int *histogram, int shmID, int *readIndex, int *secondsElapsed);

#endif
