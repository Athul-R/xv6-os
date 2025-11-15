#include "types.h"
// #include "stat.h"
#include "user.h"
// #include "fcntl.h"

#define LOW_PRIORITY 5
#define MEDIUM_PRIORITY 3
#define HIGH_PRIORITY 1
#define RESOURCE_ID 0

void low_priority_process() {
    printf(1, "[LOW] Process (PID: %d) started\n", getpid());
    lock(RESOURCE_ID);
    printf(1, "[LOW] Process (PID: %d) acquired lock\n", getpid());

    // Simulate holding the lock for a few seconds
    sleep(500);  // Adjust time if necessary

    printf(1, "[LOW] Process (PID: %d) releasing lock\n", getpid());
    release(RESOURCE_ID);
    exit();
}

void medium_priority_process() {
    printf(1, "[MEDIUM] Process (PID: %d) started\n", getpid());
    for (;;) {
        // low_priority process will never execute unless priority inheritance is used.
    }

    printf(1, "[MEDIUM] Process (PID: %d) finished work\n", getpid());
    exit();
}

void high_priority_process() {
    // Wait for a moment to ensure the low-priority process acquires the lock
    sleep(300);  // Adjust timing as necessary

    printf(1, "[HIGH] Process (PID: %d) started\n", getpid());
    lock(RESOURCE_ID);  // This should trigger priority inheritance
    printf(1, "[HIGH] Process (PID: %d) acquired lock\n", getpid());

    // Release the lock immediately
    release(RESOURCE_ID);
    printf(1, "[HIGH] Process (PID: %d) released lock\n", getpid());
    exit();
}

int main() {
    int low_pid, medium_pid, high_pid;

    // Start low-priority process
    low_pid = fork();
    if (low_pid == 0) {
        nice(getpid(), LOW_PRIORITY);
        low_priority_process();
    }

    sleep(10);
    // Start medium-priority process
    medium_pid = fork();
    if (medium_pid == 0) {
        nice(getpid(), MEDIUM_PRIORITY);
        medium_priority_process();
    }

    // Start high-priority process
    high_pid = fork();
    if (high_pid == 0) {
        nice(getpid(), HIGH_PRIORITY);
        high_priority_process();
    }

    // Wait for all processes to finish
    wait();
    wait();
    // wait();

    exit();
}
