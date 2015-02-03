#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define STRBUFFER 2048

void mytree(char *, int);

void main(int argc, char * argv[]) {
    char buf[STRBUFFER];
    if (argc <= 1)
        getcwd(buf, STRBUFFER);
    else
        strncpy(buf, argv[1], STRBUFFER);

    printf(".\n");
    mytree(buf, 1);
}

void mytree(char * directory, int level) {
    struct dirent * fileentry; 

    DIR *  currdir = opendir(directory);
    char * dirname = strcat(directory, "/");
    
    if (!currdir) return;

    while(fileentry = readdir(currdir)) {
        if (!strncmp(fileentry->d_name, ".", 1))
            continue;
        for (int i = level; i > 0; i--)
            if (i == 1)
                printf("-");
            else
                printf("+");
        printf(" %s\n", fileentry->d_name);

        if (fileentry->d_type == 4) {
            mytree(strcat(dirname, fileentry->d_name), level + 1);
        }
    }
}
