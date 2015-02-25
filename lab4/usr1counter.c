/**
 * usr1counter.c
 * Emil Tullstedt <emiltu@kth.se>
 *
 * Self-contained POSIX.1 application which ignores SIGINT and keep a counter
 * for SIGUSR1
 *
 * Compiles using ISO C11, probably compiles w/ ISO C99
 */

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int usr1_counter = 0;
void usr1a(int);
void usr1b(int);
int sigupdate(int, void *);

int main(void) {
    /**
     * Ignores SIGINT and sets the signal handler for SIGUSR1 to usr1a
     */

    if (sigupdate(SIGUSR1, &usr1a) == -1) 
        fprintf(stderr, "\nSIGUSR1 catch failed\n");

    // signal(2) manpage says it's OK to use signal() for SIG_IGN
    if (signal(SIGINT, SIG_IGN) == SIG_ERR)
        fprintf(stderr, "\nSIGINT catch failed\n");

    printf("PID: %d\n", getpid());
    while(1)
        sleep(1);
}

int sigupdate(int signo, void * fp) {
    /**
     * Redirects the signal signo to the function at memory address fp
     * Wrapper for sigaction, does what signal(2) should do
     */

    struct sigaction sa;
    sa.sa_handler = fp;

    if(sigaction(signo, &sa, NULL) == -1)
        return -1;
    return 0;
}

void usr1a(int signo) {
    /**
     * Re-allocate the signal handler for signo to usr1b()
     */
    if(sigupdate(signo, &usr1b) == -1)
        fprintf(stderr, "\nCould not rebind signal %d to usr1b\n", signo);
}

void usr1b(int signo) {
    if(signo == SIGUSR1)
        usr1_counter++;
    printf("USR1 counter now on: %d\n", usr1_counter);
}

