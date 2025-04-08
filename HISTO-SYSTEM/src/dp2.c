// FILE NAME    : dp2.c
// PROGRAMMER   : 
// DATE         : 2025-4-8
// DESCRIPTION  : 

#include "shared.h"

int main(int argc, char* argv[])
{
    printf("Hello, DP-2!\n");   // Debug message [ERASE BEFORE SUBMISSION]

    // 1. Parse shmID from argv[1]
    if (argc < 2)
    {
        fprintf(stderr, "[DP-2] Usage: %s <shmID>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int shmID = atoi(argv[1]);

    // 2. Get PID of DP-1 (getppid())
    // 3. Get PID of self (getpid())
    // 4. Fork and exec DC with shmID, DP-1 PID, DP-2 PID

    // 5. Attach to shared memory
    SharedMemory *shmPtr = (SharedMemory *)shmat(shmID, NULL, 0);
    if (shmPtr == (void *)-1)
    {
        perror("[DP-2] shmat failed");
        exit(EXIT_FAILURE);
    }

    printf("[DP-2] Successfully attached to shared memory (ID: %d)\n", shmID);  // Debug message [ERASE BEFORE SUBMISSION]

    // while (1)
    // {
        // 6. Generate 1 random letter ('A'-'T')
        // 7. Lock semaphore
        // 8. Write one letter if buffer is not full
        // 9. Update writeIndex (wrap around)
        // 10. Unlock semaphore
        // 11. Sleep 1/20 second (usleep)
    // }
    
    return 0;
}

// SIGINT handler:
// - Detach shared memory
// - Exit