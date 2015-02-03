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
    #define PERMISSION_STRING_CHARCNT 10
    struct passwd * userlst = getpwuid(fileinfo->st_uid);
    struct group * grouplst = getgrgid(fileinfo->st_gid);
    char * username;
    char * groupname;

    char rv[PERMISSION_STRING_CHARCNT + 1];
    privileges(fileinfo->st_mode, rv);

    if (userlst)
        username = userlst->pw_name;
    else
        username = "-";

    if (grouplst)
        groupname = grouplst->gr_name;
    else
        groupname = "-";

    printf("File: %s\n", file);
    printf("Size: %-8lu Blocks: %-8lu I/O Block: %-8lu\n",
        fileinfo->st_size, fileinfo->st_blocks, fileinfo->st_blksize);
    printf("Device: %#010lx Inode: %-8lu Links: %-8lu\n",
        fileinfo->st_dev, fileinfo->st_ino, fileinfo->st_nlink);
    printf("Access: (%06o / %s) UID: ( %u/ %s) GID: ( %u/ %s)\n",
        fileinfo->st_mode, rv,
        fileinfo->st_uid, username,
        fileinfo->st_gid, groupname);
    printf("Access: %s", ctime(&fileinfo->st_atim.tv_sec));
    printf("Modified: %s", ctime(&fileinfo->st_mtim.tv_sec));
    printf("Created: %s", ctime(&fileinfo->st_ctim.tv_sec));
    
}

#define EXECUTE_BIT 9
#define WRITE_BIT 8
#define READ_BIT 7
/**
 * Takes a integerversion of accesslevels and converts it to a string stored
 * in rv. The string will be characters long
 */
void privileges (int accesslevel, char * rv) {
    for(int i = 0; i < PERMISSION_STRING_CHARCNT; i++)
        rv[i] = '-';
    rv[PERMISSION_STRING_CHARCNT] = '\0'; 

    /* iterate over the access levels for user, group and everyone */
    for (int i = 0; i < 3; i++) {
        if(0x1 & accesslevel)
            rv[EXECUTE_BIT-3*i] = 'x';
        if(0x2 & accesslevel)
            rv[WRITE_BIT-3*i] = 'w';
        if(0x4 & accesslevel)
            rv[READ_BIT-3*i] = 'r';
        accesslevel = accesslevel >> 3;
    }

    // sticky bit
    if (0x1 & accesslevel)
        rv[0] = 's';
    // setgid
    if (0x2 & accesslevel && rv[6] == 'x')
        rv[6] = 's';
    // setuid
    if (0x4 & accesslevel && rv[3] == 'x')
        rv[3] = 's';
}

