#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define STRINGBUFFER 2048

void main(int argc, char * argv[]) {
    char strbuf[STRINGBUFFER];

    while(1) {
        int rv = 0;
        printf("%> ");
        fgets(strbuf, (STRINGBUFFER-1), stdin);

        if (!strcmp(strbuf, "exit\n") || !strncmp(strbuf, "exit ", 5))
        {
            printf("Shutting down...\n");
            exit(0);
        }

        if (!strncmp(strbuf, "cd ", 3)) {
            char * cwd = strbuf+3;
            char * c = strchr(cwd, '\n');

            if (c != NULL)
                *c = '\0';

            if(chdir(cwd) != 0)
                fprintf(stderr, "Cannot find directory %s\n", cwd);
        } else {
            rv = system(strbuf);
        }

        if (rv)
            printf("An error occured!\n");
    }
}

