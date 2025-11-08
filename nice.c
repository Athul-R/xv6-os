#include "types.h"
#include "user.h"

int
atoi_err(const char *s, int* err)
{
    int n;
    n = 0;
    while('0' <= *s && *s <= '9'){
        n = n*10 + *s++ - '0';
    }
    if (*s != '\0') *err = 1;
    return n;
}

int parse_args(int argc, char* argv[], int* pid, int* priority){
    if (argc == 1){
        printf(1, "Athul R ar6316\n");
        printf(1, "%s: too few arguments\n", argv[0]);
        printf(1, "Usage: %s <pid> <priority_value>\n", argv[0]);
        printf(1, "Usage: %s <priority_value>\n", argv[0]);
        return -1;
    }
    if (argc > 3){
        printf(1, "Athul R ar6316\n");
        printf(1, "%s: too many arguments\n", argv[0]);
        printf(1, "Usage: %s <pid> <priority_value>\n", argv[0]);
        printf(1, "Usage: %s <priority_value>\n", argv[0]);
        return -1;
    }
    if (argc == 2){
        int err = 0;
        *pid = getpid();
        *priority = atoi_err(argv[1], &err);
        if (err){
            printf(1, "Athul R ar6316\n");
            printf(1, "%s: priority should be a number\n", argv[0]);
            return -1;
        }
    }
    else if (argc == 3){
        int err1 = 0, err2 = 0;
        *pid = atoi_err(argv[1], &err1);
        *priority = atoi_err(argv[2], &err2);
        if (err1 || err2){
            printf(1, "Athul R ar6316\n");
            printf(1, "%s: pid and priority should be a number\n", argv[0]);
            return -1;
        }
    }
    return 0;
}

int main(int argc, char* argv[]){
    int pid = 0;
    int priority = 0;
    int old_priority = 0;
    if (parse_args(argc, argv, &pid, &priority) < 0){
        exit();
    };
    if((old_priority = nice(pid, priority)) < 0){
        printf(1, "Athul R ar6316\n");
        printf(1, "%s: nice() syscall failed\n", argv[0]);
        exit();
    }
    printf(1, "%d %d\n", pid, old_priority);
    exit();
    return 0;
}
