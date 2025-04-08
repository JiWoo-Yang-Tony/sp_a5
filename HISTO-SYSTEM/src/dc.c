#include "shared.h"

int main()
{
    printf("Hello, DC!\n");

    // 1. Parse shmID, DP-1 PID, DP-2 PID from argv[]
    // 2. Attach to shared memory
    // 3. Initialize histogram counter array [A-T]

    // 4. Set up SIGALRM every 2 seconds
    // 5. Set up SIGINT handler

    // while (1)
    // {
        // 6. On SIGALRM: Wake up and read letters
        // 7. Lock semaphore
        // 8. Read letters from buffer (up to writeIndex)
        // 9. Count each letter (A-T)
        // 10. Update readIndex (wrap around)
        // 11. Unlock semaphore

        // 12. Every 10 seconds → clear screen, print histogram
        // 13. If SIGINT received and all data read → break
    // }

    // 14. Final histogram output
    // 15. Clean up: detach shared memory and remove IPCs
    // 16. Print "Shazam !!"

    return 0;
}

// SIGALRM handler:
// - Flag to indicate reading should occur

// SIGINT handler:
// - Send SIGINT to DP-1 and DP-2
// - Set shutdown flag