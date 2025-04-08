#include "shared.h"

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

int main()
{
    printf("Hello, DP-1!\n");

    int shmID;
    int semID;
    SharedMemory *shmPtr;

    // 1. Create shared memory
    initialize_shared_memory(&shmID, &shmPtr);
    
    // 3. Create and initialize semaphore
    semID = initialize_semaphore();

    // 4. Fork and exec DP-2 with shmID as argv

    // while (1)
    // {
        // 5. Generate 20 random letters ('A'-'T')
        // 6. Lock semaphore
        // 7. Check space, write as many as possible
        // 8. Update writeIndex (wrap around if needed)
        // 9. Unlock semaphore
        // 10. Sleep 2 seconds
    // }

    return 0;
}

// SIGINT handler:
// - Detach shared memory
// - Clean up and exit