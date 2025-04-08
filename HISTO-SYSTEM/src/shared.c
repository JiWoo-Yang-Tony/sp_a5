// FILE NAME    : shared.c
// PROGRAMMER   : 
// DATE         : 2025-4-8
// DESCRIPTION  : sem_wait(): Wait (P operation)
//                sem_signal(): Signal (V operation)

#include "shared.h"

void sem_wait(int semid)
{
    struct sembuf p = { 0, -1, SEM_UNDO };
    semop(semid, &p, 1);
}

void sem_signal(int semid)
{
    struct sembuf v = { 0, 1, SEM_UNDO };
    semop(semid, &v, 1);
}