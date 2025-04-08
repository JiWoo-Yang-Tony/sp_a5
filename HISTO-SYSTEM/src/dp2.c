// FILE NAME    : dp2.c
// PROGRAMMER   : 
// DATE         : 2025-4-8
// DESCRIPTION  : 

#include "shared.h"

void write_one_letter(SharedMemory *shmPtr, int semID)
{
    // 6. Generate 1 random letter ('A'-'T')
    char letters[ALPHABET_COUNT] = {
        'A','B','C','D','E','F','G','H','I','J',
        'K','L','M','N','O','P','Q','R','S','T'
    };

    char letter = letters[rand() % ALPHABET_COUNT];

    // 7. Lock semaphore
    sem_wait(semID);

    int nextIndex = (shmPtr->writeIndex + 1) % BUF_SIZE;

    // 8. Check space, write if buffer is not full
    if (nextIndex == shmPtr->readIndex)
    {
        printf("[DP-2] buffer full, cannot write '%c'\n", letter);  // Debug message [ERASE BEFORE SUBMISSION]
        sem_signal(semID);  // Unlock semaphore
        return;
    }

    // 9. Write one letter
    shmPtr->buffer[shmPtr->writeIndex] = letter;

    // 10. Update writeIndex (wrap around)
    shmPtr->writeIndex = nextIndex;
    printf("[DP-2] wrote '%c' to buffer at index %d\n", letter, shmPtr->writeIndex);  // Debug message [ERASE BEFORE SUBMISSION]

    // 11. Unlock semaphore
    sem_signal(semID);
}

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

    // Attach to semaphore
    int semID = semget(SEM_KEY, 1, 0);
    if (semID < 0)
    {
        perror("[DP-2] semget failed");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL) + getpid());   // randomize seed with PID to avoid same sequence with DP-1

    while (1)
    {
        write_one_letter(shmPtr, semID);
        usleep(50000);  // 11. Sleep 1/20 second
    }
    
    return 0;
}

// SIGINT handler:
// - Detach shared memory
// - Exit