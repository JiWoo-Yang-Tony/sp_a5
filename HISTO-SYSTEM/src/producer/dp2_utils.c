// FILE NAME    : dp2_utils.c
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, John Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-04-11
// DESCRIPTION  : Holds the utility functions for DP-2 process
//               1. write_one_letter() - Writes a single random letter to the circular buffer
//               2. handle_sigint() - Handles SIGINT signal by detaching from shared memory

#include "dp2_utils.h"

/*
 * FUNCTION :    write_one_letter()
 * DESCRIPTION : Writes single random letter to circular buffer
 *               1. Generates random letter A-T
 *               2. Locks semaphore
 *               3. Checks buffer space
 *               4. Writes letter if space available
 *               5. Handles circular buffer wrap-around
 *               6. Unlocks semaphore
 * PARAMETERS :  shmPtr - pointer to shared memory structure
 *               semID - semaphore ID
 * RETURNS :     none
 */
void write_one_letter(SharedMemory *shmPtr, int semID)
{
    char letters[ALPHABET_COUNT] = {
        'A','B','C','D','E','F','G','H','I','J',
        'K','L','M','N','O','P','Q','R','S','T'
    };

    char letter = letters[rand() % ALPHABET_COUNT];

    sem_wait(semID);

    int nextIndex = (shmPtr->writeIndex + 1) % BUF_SIZE;

    if (nextIndex == shmPtr->readIndex)
    {
        sem_signal(semID);  
        return;
    }

    shmPtr->buffer[shmPtr->writeIndex] = letter;

    shmPtr->writeIndex = nextIndex;

    sem_signal(semID);
}

/*
 * FUNCTION :    handle_sigint()
 * DESCRIPTION : Handles SIGINT signal by detaching from shared memory
 * PARAMETERS :  sig - signal number (unused)
 * RETURNS :     none
 */
void handle_sigint(int sig)
{
    if (global_shmPtr)
    {
        shmdt(global_shmPtr);
    }
    _exit(EXIT_SUCCESS);
}