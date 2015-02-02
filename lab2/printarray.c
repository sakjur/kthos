#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void prarr(int64_t * arrv, int64_t arrc) {
    for (uint64_t i = 0; i < arrc; i++)
    {
        printf("Iterative: %li\n", arrv[i]);
    }
}

void rearr(int64_t * arrv, int64_t arrc) {
    int64_t last = arrc - 1;

    if (last < 0)
        return;

    printf("Recursive: %li\n", arrv[last]);
    rearr(arrv, last);
}

void forkarr(int64_t * arrv, int64_t arrc) {
    pid_t chld;

    if (arrc <= 0)
        return;

    if((chld = fork()) < 0) {
        fprintf(stderr, "Fork failed");
    } else if (chld == 0) {
        printf("Fork %i: %li\n", getpid(), arrv[arrc-1]);
    } else {
        forkarr(arrv, arrc - 1);
    }
}

void main(int argc, char * argv) {
    int64_t array[5] = {15,25,262,236,23};
    prarr(array, 5);
    rearr(array, 5);
    forkarr(array, 5);
    exit(0);
}

