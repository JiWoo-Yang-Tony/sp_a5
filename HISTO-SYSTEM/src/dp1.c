#include "shared.h"

int main()
{
    printf("Hello, DP-1!\n");

    // 1. Create shared memory
    // 2. Initialize circular buffer (256 chars), read/write index = 0
    // 3. Create and initialize semaphore
    // 4. Fork and exec DP-2 with shmID as argv

    // while (1)
    // {
        // 5. Generate 20 random letters ('A'-'T')
        // 6. Lock semaphore
        // 7. Check space, write as many as possible
        // 8. Update writeIndex (wrap around if needed)
        // 9. Unlock semaphore
        // 10. Sleep 2 seconds
    // }

    return 0;
}

// SIGINT handler:
// - Detach shared memory
// - Clean up and exit