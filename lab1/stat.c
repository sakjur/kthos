#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

void statPrint(char *, struct stat *);
void privileges (int, char*);

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

void statPrint(char * file, struct stat * fileinfo) {
    char rv[11];

    privileges(fileinfo->st_mode, rv);

    printf("File: %s\n", file);
    printf("Size: %-8lu Blocks: %-8lu I/O Block: %-8lu\n",
        fileinfo->st_size, fileinfo->st_blocks, fileinfo->st_blksize);
    printf("Device: %#010lx Inode: %-8lu Links: %-8lu\n",
        fileinfo->st_dev, fileinfo->st_ino, fileinfo->st_nlink);
    printf("Access: (%06o / %s) UID: ( %u/ %s) GID: ( %u/ %s)\n",
        fileinfo->st_mode, rv,
        fileinfo->st_uid, getpwuid(fileinfo->st_uid)->pw_name,
        fileinfo->st_gid, getgrgid(fileinfo->st_gid)->gr_name);
    printf("Access: %s", ctime(&fileinfo->st_atim.tv_sec));
    printf("Modified: %s", ctime(&fileinfo->st_mtim.tv_sec));
    printf("Created: %s", ctime(&fileinfo->st_ctim.tv_sec));
    
}

#define EXECUTE_CHAR 9
#define WRITE_CHAR 8
#define READ_CHAR 7
/**
 * Takes a integerversion of accesslevels and converts it to a string stored
 * in rv. The string will be characters long
 */
void privileges (int accesslevel, char * rv) {
    for(int i = 0; i < 10; i++)
        rv[i] = '-';
    rv[0xA] = '\0'; 

    /* Iterate over the access levels for user, group and everyone */
    for (int i = 0; i < 3; i++) {
        if(0x1 & accesslevel)
            rv[EXECUTE_CHAR-3*i] = 'x';
        if(0x2 & accesslevel)
            rv[WRITE_CHAR-3*i] = 'w';
        if(0x4 & accesslevel)
            rv[READ_CHAR-3*i] = 'r';
        accesslevel = accesslevel >> 3;
    }

    // Sticky bit
    if (0x1 & accesslevel)
        rv[0] = 's';
    // setgid
    if (0x2 & accesslevel && rv[6] == 'x')
        rv[6] = 's';
    // setuid
    if (0x4 & accesslevel && rv[3] == 'x')
        rv[3] = 's';
}

