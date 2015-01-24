#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#define TIMEFORMATBUFFER 127

void statPrint(char *, struct stat *);

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
        printf("Usage: ./stat.out <file>\n");
        exit(1);
    }

}

void statPrint(char * file, struct stat * fileinfo) {
    printf("File: %s\n", file);
    printf("Size: %-8u Blocks: %-8u I/O Block: %-8u\n",
        fileinfo->st_size, fileinfo->st_blocks, fileinfo->st_blksize);
    printf("Device: %#010x Inode: %-8u Links: %-8u\n",
        fileinfo->st_dev, fileinfo->st_ino, fileinfo->st_nlink);
    printf("Access: (%06o /) UID: ( %u/ ) GID: ( %u/ )\n",
        fileinfo->st_mode, fileinfo->st_uid, fileinfo->st_gid);
    printf("Access: %s", ctime(&fileinfo->st_atim.tv_sec));
    printf("Modified: %s", ctime(&fileinfo->st_mtim.tv_sec));
    printf("Created: %s", ctime(&fileinfo->st_ctim.tv_sec));
}

