#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void loop(int);

void main(int argc, char * argv[]) {
    if (argc <= 1)
        exit(EINVAL);

    int val = atoi(argv[1]);
    loop(val);
}

void print_pidprio() {
    pid_t pid = getpid();
    int prio = getpriority(PRIO_PROCESS, pid);
    printf("PID: %i, Priority: %i\n" , pid, prio);
}

void loop(int val) {
    pid_t chld;

    print_pidprio();

    if((chld = fork()) < 0) {
        fprintf(stderr, "Fork failed");
        exit(1);
    } else if (chld == 0) {
        printf("Fork'd\n");
        setpriority(PRIO_PROCESS, getpid(), val);
    } else {
        // A tiny sleep so that the fork can finish before stepping forward
        sleep(1);
    }

    print_pidprio();

    while(1) {
       // nop
    }
}
