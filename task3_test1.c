#include "types.h"
#include "user.h"
// #include "fcntl.h"

#define RESOURCE_ID 0  // The resource ID we will lock

void acquire_release_lock(void) {
    int *resource;

    // Acquire the lock
    printf(1, "Process (PID: %d) trying to acquire the lock\n", getpid());
    resource = (int*)lock(RESOURCE_ID);
    if (resource == (int*)-1) {
        printf(1, "Process (PID: %d) failed to acquire lock\n", getpid());
        exit();
    }
    printf(1, "Process (PID: %d) successfully acquired the lock\n", getpid());

    // Simulate some work while holding the lock
    sleep(100);

    // Release the lock
    printf(1, "Process (PID: %d) releasing the lock\n", getpid());
    release(RESOURCE_ID);

    exit();
}

void test_invalid_lock_id(void) {
    // Test invalid lock ID (out of range)
    int *resource = (int*)lock(RESOURCE_ID + 8);  // Invalid lock ID
    if (resource == (int*)-1) {
        printf(1, "Invalid lock ID detected, error handled correctly\n");
    } else {
        printf(1, "Test failed: Invalid lock ID should have caused an error\n");
    }
}

void waiting_for_lock(void) {
    int *resource;

    // Process will attempt to acquire the lock while it's held by another process
    printf(1, "Process (PID: %d) trying to acquire the lock and will wait\n", getpid());
    resource = (int*)lock(RESOURCE_ID);
    if (resource == (int*)-1) {
        printf(1, "Process (PID: %d) failed to acquire lock\n", getpid());
        exit();
    }
    printf(1, "Process (PID: %d) successfully acquired the lock\n", getpid());
    release(RESOURCE_ID);
}

int main(void) {
    // Test case 1: Basic Lock/Unlock functionality

    // First process: Acquire and release lock
    int pid1 = fork();
    if (pid1 == 0) {
        acquire_release_lock();
    }

    // Second process: Demonstrate wait on lock
    int pid2 = fork();
    if (pid2 == 0) {
        sleep(5);  // Delay to ensure the first process holds the lock
        waiting_for_lock();
    }

    // Wait for both child processes to finish
    wait();
    wait();

    // Test invalid lock ID
    test_invalid_lock_id();

    printf(1, "Test Case 1: Basic Lock/Unlock Complete\n");
    exit();
}
