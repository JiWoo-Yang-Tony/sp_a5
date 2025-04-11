#include "shared.h"
#include "process_utils.h"
#include "dc.h"

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

void print_histogram(int histogram[])
{
    system("clear");             
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

