#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

extern void statPrint(char *, struct stat *);

int main(int argc, char* argv[]) {
    struct stat buf;

    if (argc > 1)
    {
        int rv = stat(argv[1], &buf);
        statPrint(argv[1], &buf);
        exit(rv);
    }
    else
    {
        printf("Usage: %s <file>\n", argv[0]);
        exit(1);
    }

}

