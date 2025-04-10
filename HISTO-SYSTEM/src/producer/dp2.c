// FILE NAME    : dp2.c
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, John Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-4-8
// DESCRIPTION  : The DP-2 process will do the following:
//                              Initial Setup:
//                              1. Receives shared memory ID from DP-1 as command line argument
//                              2. Gets its own PID and parent DP-1's PID
//                              3. Forks and launches DC process, passing shmID, DP-1 PID, and DP-2 PID
//                              4. Attaches to existing shared memory segment
//                              5. Connects to existing semaphore
//
//                              Main Loop:
//                              6. Generates 1 random letter (A-T)
//                              7. Locks semaphore for atomic operation
//                              8. Checks buffer space to prevent overwriting unread data
//                              9. Writes single letter to circular buffer
//                              10. Updates write index with wrap-around at 256 elements
//                              11. Unlocks semaphore
//                              12. Sleeps for 1/20 second (50,000 microseconds)
//
//                              Outside Signals:
//                              13. If SIGINT is triggered, detaches from shared memory and exits silently

#include "shared.h"
#include "dp2.h"

// Global pointer for signal handler cleanup
static SharedMemory* global_shmPtr = NULL;


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



/*
 * FUNCTION :    main()
 * DESCRIPTION : Main DP-2 process flow:
 *               1. Parses command line args
 *               2. Launches DC process with proper command line args
 *               3. Attaches to shared memory 
 *               4. Enters production loop
 * PARAMETERS :  argc - argument count
 *               argv - argument vector (expects shmID in argv[1])
 * RETURNS :     0 on success
 */
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

    // 2. Get required PIDs
    pid_t dp1_pid = getppid();
    pid_t dp2_pid = getpid();

    // 3. Fork and launch DC process
    pid_t dc_pid = fork();
    if (dc_pid < 0)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    else if (dc_pid == 0)
    {
        char shm_str[16], dp1_str[16], dp2_str[16];
        snprintf(shm_str, sizeof(shm_str), "%d", shmID);
        snprintf(dp1_str, sizeof(dp1_str), "%d", dp1_pid);
        snprintf(dp2_str, sizeof(dp2_str), "%d", dp2_pid);

        execl("./bin/DC", "DC", shm_str, dp1_str, dp2_str, NULL);
        perror("execl failed");
        _exit(EXIT_FAILURE);
    }

    // 4. Attach to shared memory
    SharedMemory* shmPtr = (SharedMemory*)shmat(shmID, NULL, 0);
    if (shmPtr == (void*)-1)
    {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }
    global_shmPtr = shmPtr;

    // 5. Connect to semaphore
    int semID = semget(SEM_KEY, 1, 0);
    if (semID < 0)
    {
        perror("semget failed");
        exit(EXIT_FAILURE);
    }

    // Set up signal handler
    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);


    srand(time(NULL) + getpid());   // randomize seed with PID to avoid same sequence with DP-1

    while (1)
    {
        write_one_letter(shmPtr, semID);
        usleep(50000);  // 11. Sleep 1/20 second
    }
    
    return 0;
}




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


