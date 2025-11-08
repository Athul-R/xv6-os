#include "types.h"
#include "user.h"

/* Test case 3
    Create 3 processes and assign default priority of 3 to all 
    except of first process. First process will get the highest priorit of 1.
    Each process will execute the same print_with_sleep function that prints
    the pid of the process and next integer. It prints in the infinite loop with sleeping
    for 30 seconds in between each iteration. 
    Will execute this test with two make options: 
    1. Use default Round Robin scheduler. No extra arguments to make qemu-nox
    2. Use new Priority scheduler. Extra arguments PRR=1 CPUS=1 to make qemu-nox.
    In the first case of Round Robin scheduler, we should see equally distributed prints among
    the 3 processes. 
    In the second case of Priority schedule, even though we assigned highest priority to the first
    process, we should still see equally distributed prints among the 3 processes. That is because 
    the highest priority process sleeps 30 seconds in between each iteration , during which time
    the CPU can switch the context to other processes. Unlike in Test case 2, where we did 
    not sleep in between each iteration, so that CPU was always serving the highest prioriy process. 
*/

void print_with_sleep(int pid)
{
    int i;
    for (i = 0;i<50;i++) {
        sleep(30);
        printf(1,"Athul R ar6316\npid %d, i %d.\n", pid, i);
    }
}

int main()
{
    int pid;
    int k,n;
    int priority = 3; // default priority
    n=3; // 3 processes
    
    for (k=0;k<n;k++) {
        pid=fork();
        if(pid>0) {
            if (k==0)
                priority = 1;
            else 
                priority = 3;
            nice(pid, priority);
            printf(1, "Athul R ar6316\nk %d - Parent %d creating child %d with priorty %d\n",k, getpid(), pid, priority);
        }
        else{
            int child_pid = getpid();
	        printf(1,"Athul R ar6316\nChild %d created\n",child_pid);
            print_with_sleep(child_pid);
        }
    }
    exit();
    return 0;
}
