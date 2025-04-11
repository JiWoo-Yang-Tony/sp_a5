// FILE NAME    : dc.c
// PROGRAMMER   : Josh Horsley, Kalina Cathcart, Jon Paventi, Daimon Quin, Jiwoo Yang
// DATE         : 2025-4-8
// DESCRIPTION  :

#include "shared.h"
#include "dc.h"


volatile sig_atomic_t readFlag = 0;
volatile sig_atomic_t shutdownFlag = 0;
pid_t dp1PID = -1;
pid_t dp2PID = -1;

int semID = -1;
SharedMemory *shmPtr = NULL; 

int main(int argc, char *argv[])
{
    printf("Hello, DC!\n");
    
    int shmID = -1;
    int *histogram;
    int readIndex = 0;
    int secondsElapsed = 0;

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
    histogram = (int*)calloc(ALPHABET_COUNT, sizeof(int));
    

    // 4. Set up SIGALRM every 2 seconds
    // struct sigaction sa_alarm, sa_int;
    // sa_alarm.sa_handler = sigalarm_handler;
    // sigemptyset(&sa_alarm.sa_mask);
    // sa_alarm.sa_flags = 0;

    // sa_int.sa_handler = sigint_handler;
    // sigemptyset(&sa_int.sa_mask);
    // sa_int.sa_flags = 0;

    // sigaction(SIGALRM, &sa_alarm, NULL);
    // sigaction(SIGINT, &sa_int, NULL);


    // 5. Set up SIGINT handler
    signal(SIGINT, sigint_handler);
    signal(SIGALRM, sigalarm_handler);
    //startTime = time(NULL);
    alarm(2);

    run_consumer_loop(histogram, shmID, &readIndex, &secondsElapsed);

    // 14. Final histogram output
    print_histogram(histogram);

    return 0;
}

void print_histogram(int histogram[])
{
    //system("clear");              //commented out for debug
    printf("Histogram [A-T]:\n");
    for (int i = 0; i < ALPHABET_COUNT; i++) 
    {
        char letter = 'A' + i;
        int counter = histogram[i];
        printf("%c: %d\n", letter, counter); //DEBUG dont believe we want counters

        int asterisks = counter / 100;
        int plus = (counter % 100) / 10;
        int minus = counter % 10;

        for (int j = 0; j < asterisks; j++) 
        {
            printf("*");
        }
        for (int j = 0; j < plus; j++) 
        {
            printf("+");
        }
        for (int j = 0; j < minus; j++) 
        {
            printf("-");
        }
        printf("\n");
    }
    printf("--------------------------\n");
}

// SIGALRM handler:
// - Flag to indicate reading should occur
void sigalarm_handler(int signal) 
{
    readFlag = 1;
}

void sigint_handler(int signal) 
{
    shutdownFlag = 1;
    //Send SIGINT to DP-1 and DP-2
    kill(dp1PID, SIGINT);
    kill(dp2PID, SIGINT);
}

void run_consumer_loop(int *histogram, int shmID, int *readIndex, int *secondsElapsed) 
{
    while (1) 
    {
        pause();

        if (!readFlag) continue;
        readFlag = 0;

        sem_wait(semID);
        int writeIndex = shmPtr->writeIndex;

        for (int i = 0; i < READ_AMOUNT; i++) 
        {
            if (*readIndex == writeIndex) break;

            char letter = shmPtr->buffer[*readIndex];
            if (letter >= 'A' && letter <= 'T') 
            {
                histogram[letter - 'A']++;
            }
            *readIndex = (*readIndex + 1) % BUF_SIZE;
            shmPtr->readIndex = *readIndex;
        }
        sem_signal(semID);

        *secondsElapsed += 2;
        if (*secondsElapsed % HISTOGRAM_INTERVAL == 0) 
        {
            print_histogram(histogram);
        }

        if (shutdownFlag && *readIndex == shmPtr->writeIndex) 
        {
            print_histogram(histogram);
            shmdt(shmPtr);
            shmctl(shmID, IPC_RMID, NULL);
            semctl(semID, 0, IPC_RMID);
            printf("Shazam !!\n");
            exit(0);
        }

        alarm(2);
    }
}