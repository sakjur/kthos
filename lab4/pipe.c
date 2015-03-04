#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    int pipefd[2];
    pid_t pid;
    pid_t pid2;

    if (pipe(pipefd) == -1) {
        fprintf(stderr, "Error: Pipe creation failed");
        exit(-1);
    }

    pid = fork();
    if (pid == -1) {
        fprintf(stderr, "Error: Fork failed");
    } else if (pid == 0) {
        close(0);
        close(pipefd[0]); // Close read
        dup2(pipefd[1], 1); // pipe out -> stdin
        close(pipefd[1]); // Close write

        execlp("/usr/bin/ls", "ls", "-la", (char*)NULL);
        exit(0);
    } else {
        pid2 = fork();
        if (pid2 == -1) {
            fprintf(stderr, "Error: Fork 2 failed");
            exit(-1);
        } else if (pid2 == 0) {
            close(pipefd[1]); // Close write
            dup2(pipefd[0], 0); // pipe in -> stdout
            close(pipefd[0]); // Close read 
            execlp("/usr/bin/less", "less", (char*)NULL);
            exit(0);
        }
        
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(pid, NULL, 0);
        waitpid(pid2, NULL, 0);
        exit(0);
    }
}

