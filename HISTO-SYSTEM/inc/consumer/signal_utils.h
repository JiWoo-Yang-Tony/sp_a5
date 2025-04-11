#ifndef SIGNAL_UTILS_H
#define SIGNAL_UTILS_H

#include <signal.h>

void sigalarm_handler(int sig);
void sigint_handler(int sig);

#endif
