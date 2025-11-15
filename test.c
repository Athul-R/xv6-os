#include "types.h"
#include "user.h"

void print_prime(int pid)
{
    int i=0;
    int j=0;
    int prime = 0;
    for (i = 0;;i++) {
        prime = 1;
        for(j = 2;j<i;j++) {
            if (i % j == 0) {
                prime = 0;
                break;
            }
        }
        if (prime)
            printf(1,"pid %d ,  prime %d\n", pid, i);
    }
}

int main()
{
    int pid;
    int k,n;
    int priority = 3;
    n=5;
    for (k=0;k<n;k++) {
        pid=fork();
        if(pid>0) {
            if (k==0)
                priority = 1;
            else 
                priority = 3;
            nice(pid, priority);
            printf(1, "k %d Parent %d creating child %d with  priorty %d\n",k, getpid(), pid, priority);

            //wait();    
        }
        else{
	        printf(1,"Child %d created\n",getpid());
            print_prime(getpid());
        }
    }
    exit();
    return 0;
}
