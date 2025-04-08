// FILE NAME    : dp1.c
// PROGRAMMER   : 
// DATE         : 2025-4-8
// DESCRIPTION  : 

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

void write_random_letters(SharedMemory *shmPtr, int semID)
{
    // 5. Generate 20 random letters ('A'-'T')
    char letters[ALPHABET_COUNT] = {
        'A','B','C','D','E','F','G','H','I','J',
        'K','L','M','N','O','P','Q','R','S','T'
    };

    for (int i = 0; i < 20; i++)
    {
        char letter = letters[rand() % ALPHABET_COUNT];

        // 6. Lock semaphore
        sem_wait(semID);

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
    }
}

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

    srand(time(NULL));  // Seed random number generator

    while (1)
    {
        write_random_letters(shmPtr, semID);       
        sleep(2);   // 10. Sleep 2 seconds
    }

    return 0;
}

// SIGINT handler:
// - Detach shared memory
// - Clean up and exit