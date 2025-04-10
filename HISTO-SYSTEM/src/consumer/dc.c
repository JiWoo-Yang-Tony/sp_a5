// FILE NAME    : dc.c
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, Jon Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-4-8
// DESCRIPTION  :

#include "shared.h"
#include "dc.h"

volatile sig_atomic_t readFlag = 0;
pid_t dp1PID = -1;
pid_t dp2PID = -1;

int main(int argc, char *argv[])
{
    printf("Hello, DC!\n");
    SharedMemory *shmPtr = NULL; 
    int semID = -1;
    int shmID = -1;
    int histogram[ALPHABET_COUNT] = {0};
    time_t startTime;

    // 1. Parse shmID, DP-1 PID, DP-2 PID from argv[]
    if(argc < 4)
    {
        fprintf(stderr, "[DC] Usage: %s <shmID> <DP_PID> <DP2_PID>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    shmID = atoi(argv[1]);
    dp1PID = atoi(argv[2]);
    dp2PID = atoi(argv[3]);

    // 2. Attach to shared memory
    shmPtr = (SharedMemory*)shmat(shmID, NULL, 0);
    if(shmPtr == (void*)-1)
    {
        perror("[DC] shmat failed.\n");
        exit(EXIT_FAILURE);
    }

    // Attach to semaphore
    semID = semget(SEM_KEY, 1, 0);
    if(semID < 0)
    {
        perror("[DC] semget failed.\n");
        exit(EXIT_FAILURE);
    }

    // 3. Initialize histogram counter array [A-T]
        // Not sure how to do this

    // 4. Set up SIGALRM every 2 seconds
    struct sigaction sa_alarm, sa_int;
    sa_alarm.sa_handler = sigalarm_handler;
    sigemptyset(&sa_alarm.sa_mask);
    sa_alarm.sa_flags = 0;

    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;

    sigaction(SIGALRM, &sa_alarm, NULL);
    sigaction(SIGINT, &sa_int, NULL);


    // 5. Set up SIGINT handler
    signal(SIGINT, sigint_handler);

    startTime = time(NULL);
    alarm(2);

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
                if(letter >= 'A' && letter <= 'T')
                {
                    histogram[letter - 'A']++;
                }

                // 10. Update readIndex (wrap around) -- Not sure if this will work
                shmPtr->readIndex = (shmPtr->readIndex + 1) % BUF_SIZE;
            }

            // 11. Unlcok semaphore
            sem_signal(semID);

            // 12. Every 10 seconds -> clear screen, print histogram
            time_t elapsed = time(NULL) - startTime;
            if(elapsed % 10 == 0)
            {
                print_histogram(histogram);
            }
        }

        // 13. If SIGINT received and all data read -> break
        if(shutdown_flag && shmPtr->readIndex == shmPtr->writeIndex)
        {
            break;
        }

        usleep(50000);
    }

    // 14. Final histogram output
    print_histogram(histogram);

    // 15. Clean up: detach shared memory and remove IPCs
    shmdt(shmPtr);
    shmctl(shmID, IPC_RMID, NULL);  // I think the DC does this?
    semctl(semID, 0, IPC_RMID);     // Remove semaphore

    // 16. Print "Shazam !!"
    printf("Shazam! It has all worked so far!\n");

    return 0;
}

void print_histogram(int histogram[])
{
    system("clear");
    printf("Histogram [A-T]:\n");
    for (int i = 0; i < ALPHABET_COUNT; i++) 
    {
        char letter = 'A' + i;
        printf("%c: %d\n", letter, histogram[i]);
    }
    printf("--------------------------\n");
}

// SIGALRM handler:
// - Flag to indicate reading should occur
void sigalarm_handler(int signal) 
{
    //alarm_flag = 1;
    alarm(2); // re-arm the alarm
}

void sigint_handler(int signal) 
{
    shutdown_flag = 1;
    //Send SIGINT to DP-1 and DP-2
    kill(dp1PID, SIGINT);
    kill(dp2PID, SIGINT);
}