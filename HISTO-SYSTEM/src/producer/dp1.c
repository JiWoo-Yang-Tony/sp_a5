// FILE NAME    : dp1.c
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, John Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-4-8
// DESCRIPTION  : This program will do the following: 
//                              Initial Setup: 
//                              1. Checks for shared memory, if not found, it will create the shared memory for the circular buffer
//                                      This program is solely responsible for generating the shared memory key 
//                              2. When the DP-1 program has attatched to the shared memory, it will initialize the shared memory elements
//                              3. Launches it's fellow DP-2 process through a fork() call, passing only the shared memory ID as its command line args 
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

 // Global pointer for signal handler cleanup (necessary evil)
SharedMemory* global_shmPtr = NULL;

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

    int shmID;
    int semID;
    SharedMemory *shmPtr;

    struct sigaction sa;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    initialize_shared_memory(&shmID, &shmPtr);
       
    semID = initialize_semaphore();


   pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        char shmStr[16];
        snprintf(shmStr, sizeof(shmStr), "%d", shmID);

        char *args[] = { "./bin/DP-2", shmStr, NULL };
        execvp(args[0], args);

        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else {
    }

	srand(time(NULL));

    while (1)
    {
		write_random_letters(shmPtr, semID); 
		sleep(2);    

    }


    return 0;
}










 










