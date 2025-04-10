// FILE NAME    : shared.c
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, Jon Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-4-8
// DESCRIPTION  : Implements shared functionality for all processes

#include "../inc/shared.h"

// Global signal handling flag
volatile sig_atomic_t shutdown_flag = 0;

void sem_wait(int semid)
{
    struct sembuf p = { 0, -1, SEM_UNDO };
    if (semop(semid, &p, 1) == -1) {
        if (errno != EINTR) { // Ignore interruptions by signals
            perror("sem_wait failed");
            exit(EXIT_FAILURE);
        }
    }
}

void sem_signal(int semid)
{
    struct sembuf v = { 0, 1, SEM_UNDO };
    if (semop(semid, &v, 1) == -1) {
        perror("sem_signal failed");
        exit(EXIT_FAILURE);
    }
}