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
SharedMemory* global_shmPtr = NULL;






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

    if (argc < 2)
    {
        fprintf(stderr, "[DP-2] Usage: %s <shmID>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int shmID = atoi(argv[1]);

    pid_t dp1_pid = getppid();
    pid_t dp2_pid = getpid();

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

    SharedMemory* shmPtr = (SharedMemory*)shmat(shmID, NULL, 0);
    if (shmPtr == (void*)-1)
    {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }
    global_shmPtr = shmPtr;

    int semID = semget(SEM_KEY, 1, 0);
    if (semID < 0)
    {
        perror("semget failed");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);


    srand(time(NULL) + getpid());   

    while (1)
    {
        write_one_letter(shmPtr, semID);
        usleep(50000);
    }
    
    return 0;
}

