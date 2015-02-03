#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRINGBUFFER 2048

void main(int argc, char * argv[]) {
    char strbuf[STRINGBUFFER];

    while(1) {
        printf("%> ");
        fgets(strbuf, (STRINGBUFFER-1), stdin);

        if (!strncmp(strbuf, "exit", 4))
        {
            printf("Shutting down...\n");
            exit(0);
        }

        int rv = system(strbuf);

        if (rv)
            printf("An error occured!\n");
    }
}

