#include "shared.h"

int main(int argc, char* argv[])
{
    printf("Hello, DP-2!\n");

    // 1. Parse shmID from argv[1]
    // 2. Get PID of DP-1 (getppid())
    // 3. Get PID of self (getpid())
    // 4. Fork and exec DC with shmID, DP-1 PID, DP-2 PID

    // 5. Attach to shared memory

    // while (1)
    // {
        // 6. Generate 1 random letter ('A'-'T')
        // 7. Lock semaphore
        // 8. Write one letter if buffer is not full
        // 9. Update writeIndex (wrap around)
        // 10. Unlock semaphore
        // 11. Sleep 1/20 second (usleep)
    // }
    
    return 0;
}

// SIGINT handler:
// - Detach shared memory
// - Exit