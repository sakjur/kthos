#include <dirent.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

#define STRBUFFER 2048
#define TYPE_DIR 4

void mytree(char *, int);

void main(int argc, char * argv[]) {
    char buf[STRBUFFER];
    if (argc <= 1)
        getcwd(buf, STRBUFFER);
    else
        strncpy(buf, argv[1], STRBUFFER);

    printf("%s\n.\n", buf);
    mytree(buf, 1);
}

void mytree(char * directory, int level) {
    struct dirent * fileentry; 

    DIR  * currdir = opendir(directory);
    char dirname[STRBUFFER];
    strncpy(dirname, directory, STRBUFFER);
    strcat(dirname, "/");

    if (!currdir)
        return;

    while(fileentry = readdir(currdir)) {
        char tmpdir[STRBUFFER];
        strncpy(tmpdir, dirname, STRBUFFER);
        
        if (!strncmp(fileentry->d_name, ".", 1))
            continue;
        for (int i = level; i > 0; i--)
            if (i == 1)
                printf("-");
            else if (i == 2)
                printf("|");
            else
                printf(" ");

        if (fileentry->d_type == TYPE_DIR) {
            printf(" %s/\n", fileentry->d_name);
            mytree(strcat(tmpdir, fileentry->d_name), level + 1);
        } else {
            printf(" %s\n", fileentry->d_name);
        }
    }

    closedir(currdir);
}

