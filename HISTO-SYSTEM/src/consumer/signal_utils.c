#include "shared.h"
#include "signal_utils.h"
#include "dc.h"

void sigalarm_handler(int signal) 
{
    readFlag = 1;
}

void sigint_handler(int signal) 
{
    shutdownFlag = 1;
    kill(dp1PID, SIGINT);
    kill(dp2PID, SIGINT);
}