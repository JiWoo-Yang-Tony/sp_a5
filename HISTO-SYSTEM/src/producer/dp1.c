// FILE NAME    : dp1.c
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, John Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-4-8
// DESCRIPTION  : This program will do the following: 
//                              Initial Setup: 
//                              1. Checks for shared memory, if not found, it will create the shared memory for the circular buffer
//                                      This program is solely responsible for generating the shared memory key 
//                              2. When the DP-1 program has attatched to the shared memory, it will initialize the shared memory elements
//                              3. Launches it's fellow DP-2 process through a fork() call, passing only the shared memory ID and command line args 
//                              
//                              Main Loop: 
//                              4.DP-1 will check the circular buffer to see how much room is available.
//                                      If there is not enough room available, DP-1 will determine how much it can write, and only write in that amount into the circular buffer.   
//                              5. DP-1 will generate up to 20 random letters and write them into the buffer
//                                      Consider to be an atomic operation, sephamore used to safely access the write memory.
//                                      If the process writes to the 256th element in the buffer, it ensures it will wrap around to the initial write position (0) before continuing to write   
//                                      The process ensures it never writes past the circular buffer's readIndex, used by DC.
//                                      If the program detects that it has caught up to the readIndex of the buffer, then DP-1 will enter sleep mode to allow the DC time to process 
//                              6. The process will then sleep for 2 seconds and loop to check the circular buffer space once more. 
//
//                              Outside Signals: 
//                              7. If SIGINT is triggered, the process will detach itself from the shared memory and exit with no statement
//   



#include "shared.h"
#include "dp1.h"
#include <signal.h>

/*
 * FUNCTION :    main()
 * DESCRIPTION : This function holds the main logic flow for the DP-1 process.
 *               Initialization steps are taken to establish connection to the shared memory, initialize the circular buffer, and prepare the sephamore
 *               DP-2 is then launched with fork().
 *               The main loop is then run, where the program will attempt to write 20 char into the circular buffer and then sleep for 2 seconds before trying again
 * PARAMETERS :  none
 * RETURNS :     0 for successful exit
 */

int main()
{
    printf("Hello, DP-1!\n");   // Debug message [ERASE BEFORE SUBMISSION]

    int shmID;
    int semID;
    SharedMemory *shmPtr;

    // 1. Create shared memory
    initialize_shared_memory(&shmID, &shmPtr);
    
    // 2. Initialize circular buffer inside initialize_shared_memory()
    
    
    // 3. Create and initialize semaphore
    semID = initialize_semaphore();

    // 4. Fork and exec DP-2 with shmID as argv
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char shmStr[16];
        snprintf(shmStr, sizeof(shmStr), "%d", shmID);

        // Child process: exec DP-2
        char *args[] = { "./bin/DP-2", shmStr, NULL };
        execvp(args[0], args);

        // If execvp fails, error handling
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
        printf("[DP-1] Forked DP-2 with PID %d\n", pid);    // Debug message [ERASE BEFORE SUBMISSION]
    }

    //srand(time(NULL));  // Seed random number generator

   // while (1)
  //  {
   //     write_random_letters(shmPtr, semID);       
  //      sleep(2);   // 10. Sleep 2 seconds
   // }

    return 0;
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
 */ /

void initialize_shared_memory(int *shmID, SharedMemory **shmPtr)
{
    // Create shared memory
    *shmID = shmget(SHM_KEY, sizeof(SharedMemory), IPC_CREAT | 0666);
    if (*shmID < 0)
    {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }

    // Attach to shared memory
    *shmPtr = (SharedMemory *)shmat(*shmID, NULL, 0);
    if (*shmPtr == (void *)-1)
    {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    // 2. Initialize circular buffer (256 chars), read/write index = 0
    memset((*shmPtr)->buffer, 0, BUF_SIZE);
    (*shmPtr)->readIndex = 0;
    (*shmPtr)->writeIndex = 0;

    printf("[DP-1] Shared memory initialized.\n");  // Debug message [ERASE BEFORE SUBMISSION]
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
    // Create and initialize semaphore
    int semID = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semID < 0)
    {
        perror("semget failed");
        exit(EXIT_FAILURE);
    }

    // Initialize semaphore to 1 (binary semaphore)
    if (semctl(semID, 0, SETVAL, 1) == -1)
    {
        perror("semctl SETVAL failed");
        exit(EXIT_FAILURE);
    }

    printf("[DP-1] Semaphore initialized.\n");  // Debug message [ERASE BEFORE SUBMISSION]
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
 */ /

void write_random_letters(SharedMemory *shmPtr, int semID)
{








    /*

    // 5. Generate 20 random letters ('A'-'T')
    char letters[ALPHABET_COUNT] = {
        'A','B','C','D','E','F','G','H','I','J',
        'K','L','M','N','O','P','Q','R','S','T'
    };

    for (int i = 0; i < 20; i++)
    {
        char letter = letters[rand() % ALPHABET_COUNT];

        // 6. Lock semaphore
        sem_wait(semID);                                                            // TODO : This currently uses the sephamore every letter, requirements want the letters written at once, we need to add validations and checks to make sure we do not overwrite the buffer before writing letters in. 

        int nextIndex = (shmPtr->writeIndex + 1) % BUF_SIZE;

        // 7. Check space, write as many as possible
        if (nextIndex == shmPtr->readIndex)
        {
            printf("[DP-1] buffer full, cannot write '%c'\n", letter);  // Debug message [ERASE BEFORE SUBMISSION]
            sem_signal(semID);  // Unlock semaphore
            break;
        }
        shmPtr->buffer[shmPtr->writeIndex] = letter;

        // 8. Update writeIndex (wrap around if needed)
        shmPtr->writeIndex = nextIndex;

        // 9. Unlock semaphore
        sem_signal(semID);

        */
    }
}


/*
 * FUNCTION :    signal_handler()
 * DESCRIPTION : This function handles the SIGINT signal.
 *               It detaches from shared memory and cleans up before exiting.
 * PARAMETERS :  signum - signal number
 * RETURNS :     none
 */

// SIGINT handler:
// - Detach shared memory
// - Clean up and exit
