#include "types.h"
#include "user.h"

/* Test case 1
    Create 5 processes and assign default priority of 3 to all 
    except of the first process. First process will get the highest priority of 1.
    Each process will execute the same print_prime function that prints prime 
    numbers in infinite loop. The prrint_prime also prints the PID of the process
    along with the primary number. 
    Will execute this test with two make options: 
    1. Use default Round Robin scheduler. No extra arguments to make qemu-nox
    2. Use new Priority scheduler. Extra argument PRR=1 to make qemu-nox. 
        Also we will run two scnesrios:
        a)CPUS=2 
        b)CPUS=1 - which will use only one CPU to make the results more pronounced.
       make qemu-nox PRR=1 CPUS=1
    In the first case of Round Robin scheduler, we should see equally distributed prints among
    the 5 processes. 
    In the second case of Priority schedule, we should see most of the time the 
    print of the process that was assigned highest priority.
*/

void print_prime(int pid)
{
    int i=0;
    int j=0;
    int prime = 0;
    for (i = 0;i<100;i++) {
        prime = 1;
        for(j = 2;j<i;j++) {
            if (i % j == 0) {
                prime = 0;
                break;
            }
        }
        if (prime)
            printf(1,"Athul R ar6316\npid %d, prime %d.\n", pid, i);
    }
}

int main()
{
    int pid;
    int k,n;
    int priority = 3; // default priority
    n=5; // 5 processes
    
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
	        printf(1,"Child %d created\n",child_pid);
            print_prime(child_pid);
        }
    }
    exit();
    return 0;
}
