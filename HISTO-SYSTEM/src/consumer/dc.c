// FILE NAME    : dc.c
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, Jon Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-4-8
// DESCRIPTION  :

#include "shared.h"
#include "dc.h"


volatile sig_atomic_t readFlag = 0;
volatile sig_atomic_t shutdownFlag = 0;
pid_t dp1PID = -1;
pid_t dp2PID = -1;

int semID = -1;
SharedMemory *shmPtr = NULL; 

int main(int argc, char *argv[])
{
    
    int shmID = -1;
    int *histogram;
    int readIndex = 0;
    int secondsElapsed = 0;

    if(argc < 4)
    {
        fprintf(stderr, "[DC] Usage: %s <shmID> <DP_PID> <DP2_PID>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    shmID = atoi(argv[1]);
    dp1PID = atoi(argv[2]);
    dp2PID = atoi(argv[3]);

    shmPtr = (SharedMemory*)shmat(shmID, NULL, 0);
    if(shmPtr == (void*)-1)
    {
        perror("[DC] shmat failed.\n");
        exit(EXIT_FAILURE);
    }

    semID = semget(SEM_KEY, 1, 0);
    if(semID < 0)
    {
        perror("[DC] semget failed.\n");
        exit(EXIT_FAILURE);
    }

    histogram = (int*)calloc(ALPHABET_COUNT, sizeof(int));
    
    signal(SIGINT, sigint_handler);
    signal(SIGALRM, sigalarm_handler);
    alarm(2);

    run_consumer_loop(histogram, shmID, &readIndex, &secondsElapsed);

    print_histogram(histogram);

    return 0;
}


