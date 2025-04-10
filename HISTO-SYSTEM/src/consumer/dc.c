// FILE NAME    : dc.c
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, Jon Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-4-8
// DESCRIPTION  :

#include "shared.h"
#include "dc.h"

volatile sig_atomic_t readFlag = 0;
volatile sig_atomic_t shutdownFlag = 0;

int main(int argc, char *argv[])
{
    printf("Hello, DC!\n");
    SharedMemory shmPtr; 
    int semID;
    int shmID;
    int histogram[ALPHABET_COUNT] = {0};
    int doILoop = 0;
    time_t startTime; 

    // 1. Parse shmID, DP-1 PID, DP-2 PID from argv[]
    if(arc < 4)
    {
        fprintf(stderr, "[DC] Usage: %s <shmID> <DP_PID> <DP2_PID>\n", argv[0])
        exit(EXIT_FAILURE);
    }

    // 2. Attach to shared memory
    shmPtr = (SharedMemory*)shmat(shmID, NULL, 0);
    if(shmPtr == (void*)-1)
    {
        perror("[DC] shmat failed.\n");
        exit(EXIT_FAILURE);
    }

    // 3. Initialize histogram counter array [A-T]
        // Not sure how to do this

    // 4. Set up SIGALRM every 2 seconds
    signal(SIGALRM, sigalarm_handler);
    struct itimerval timer;
    timer.it_interval.tv_sec = 2;
    timer.it_interval.tv_usec = 0;
    timer.it_value = timer.it_interval;
    setitimer(ITIMER_REAL, &timer, NULL);

    // 5. Set up SIGINT handler
    signal(SIGINT, sigint_hander);

    // Get the semaphore
    semID = semget(SEM_KEY, 1, 0);
    if(semID < 0)
    {
        perror("[DC] semget failed.\n");
        exit(EXIT_FAILURE);
    }

    startTime = time(NULL);

    while(1)
    {
        // 6. On SIGALARM: Wake up and read letters
        if(readFlag)
        {
            readFlag = 0; 

            // 7. Lock Semaphore
            sem_wait(semID);

            // 8. Read letter from buffer (up to writeIndex)
            while(shmPtr->readIndex != shmPtr->writeIndex)
            {
                char letter = shmPtr->buffer[shmPtr->readIndex];
                // 9. Count each letter (A-T)
                if(letter >= 'A' && leter <= 'T')
                {
                    histogram[letter - 'A']++
                }

                // 10. Update readIndex (wrap around) -- Not sure if this will work
                shmPtr->readIndex = (shmPtr->readIndex + 1) % BUF_SIZE;
            }

            // 11. Unlcok semaphore
            sem_signal(semID);

            // 12. Every 10 seconds -> clear screen, print histogram
            if((time(NULL) - startTime) % 10 == 0)
            {
                system("clear");
                printf("[DC] Letter Histogram:\n");
                for(int i; i < ALPHABET_COUNT; i++)
                {
                    printf("%c: %d\n", 'A'+i, histogram[i]);
                }
            }
        }

        // 13. If SIGINT received and all data read -> break
        if(shutdownFlag && shmPtr->readIndex == shmPtr->writeIndex)
        {
            break;
        }

        usleep(50000);
    }

    // 14. Final histogram output
    printf("\n[DC] Final Letter Histogram:\n");
    for(int i; i <ALPHABET_COUNT; i++)
    {
        printf("%c: %d\n", 'A'+i, histogram[i]);
    }

    // 15. Clean up: detach shared memory and remove IPCs
    shmdt(shmPtr);
    shmctl(shmID, IPC_RMID, NULL);  // I think the DC does this?
    semctl(semID, 0, IPC_RMID);     // Remove semaphore

    // 16. Print "Shazam !!"
    printf("Shazam! It has all worked so far!\n");

    return 0;
}

// SIGALRM handler:
// - Flag to indicate reading should occur
void sigalarm_handler(int signal)
{
    readFlag = 1;
}

// SIGINT handler:
// - Send SIGINT to DP-1 and DP-2
// - Set shutdown flag
void sigint_handler(int signal)
{
    shutdownFlag = 1;
    kill(dp1PID, SIGINT);
    kill(dp2PID, SIGINT);
}