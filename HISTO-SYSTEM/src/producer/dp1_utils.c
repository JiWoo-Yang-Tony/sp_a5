// FILE NAME    : x
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, John Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-04-11
// DESCRIPTION  : x

#include "dp1_utils.h"


/*
 * FUNCTION :    signal_handler()
 * DESCRIPTION : This function handles the SIGINT signal.
 *               It detaches from shared memory and cleans up before exiting.
 * PARAMETERS :  signum - signal number
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

/*
 * FUNCTION :    initialize_shared_memory()
 * DESCRIPTION : This function initializes the shared memory segment and circular buffer.
 *               1. It checks if a shared memory exists, if not, it creates one.
 *               2. Regardless, it will create a shared memory key 
 *               3. It initializes the circular buffer and sets the read/write indices to 0.
 *               4. It attatches the process to the shared memory and initialized the elements within 
 *               5. The program will exit on success or failure
 * PARAMETERS :  shmID - pointer to shared memory ID
 *               shmPtr - pointer to SharedMemory structure
 * RETURNS :     none
 */

 void initialize_shared_memory(int *shmID, SharedMemory **shmPtr)
 {
     *shmID = shmget(SHM_KEY, sizeof(SharedMemory), IPC_CREAT | 0666);
     if (*shmID < 0)
     {
         perror("shmget failed");
         exit(EXIT_FAILURE);
     }
 
     *shmPtr = (SharedMemory *)shmat(*shmID, NULL, 0);
     if (*shmPtr == (void *)-1)
     {
         perror("shmat failed");
         exit(EXIT_FAILURE);
     }
 
     memset((*shmPtr)->buffer, 0, BUF_SIZE);
     (*shmPtr)->readIndex = 0;
     (*shmPtr)->writeIndex = 0;
 }

 /*
 * FUNCTION :    initialize_semaphore()
 * DESCRIPTION : This function initializes the semaphore for synchronization.
 * 
 * PARAMETERS :  none
 * RETURNS :     semID - semaphore ID, initialized to 1 (unlocked state) 
 * 
 */

int initialize_semaphore()
{
    int semID = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semID < 0)
    {
        perror("semget failed");
        exit(EXIT_FAILURE);
    }

    if (semctl(semID, 0, SETVAL, 1) == -1)
    {
        perror("semctl SETVAL failed");
        exit(EXIT_FAILURE);
    }
    return semID;
}

/*
* FUNCTION :    write_random_letters()
* DESCRIPTION : This function generates random letters and writes them to the circular buffer.
*
*                    1. The buffer is locked with the semaphore to ensure exclusive access
*                    2. It checks the circular buffer's remaining space, if space for 20 char, proceeds normally, if not, it will determine and define how many can be inserted before wrap around
*                    5. The function will check the circular buffer's readIndex to ensure it does not overwrite any data that has not been read yet, if it will surpass the readIndex, function is cancelled, returns to main loop to sleep and try again
* 				     2. It generates 20 random letters from 'A' to 'T' into a string to be passed into the circular buffer
*                    4. The function will write up to the 256th element in the buffer, and wrap around to the 0th element if needed before finishing all 20 char into the buffer
*                    5. The function will unlock the semaphore and return to the main loop to sleep and loop again 
* PARAMETERS :  shmPtr - pointer to SharedMemory structure
*               semID - semaphore ID for synchronization
* RETURNS :     none
 */

 void write_random_letters(SharedMemory *shmPtr, int semID)
 {
 
 
      const char letters[ALPHABET_COUNT] = {
        'A','B','C','D','E','F','G','H','I','J',
        'K','L','M','N','O','P','Q','R','S','T'
      };
 
      sem_wait(semID);
 
      int available = (shmPtr->readIndex - shmPtr->writeIndex - 1 + BUF_SIZE) % BUF_SIZE;
 
 
 
      int to_write = (available < 20) ? available : 20;
 
      for (int i = 0; i < to_write; i++) 
      {
          shmPtr->buffer[shmPtr->writeIndex] = letters[rand() % ALPHABET_COUNT];
 
          shmPtr->writeIndex = (shmPtr->writeIndex + 1) % BUF_SIZE;
      }
 
      sem_signal(semID);
 
 
      return; 
 
 }