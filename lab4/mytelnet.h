#ifndef MYTELNET_H
#define MYTELNET_H

#include <arpa/inet.h>

#define SERV_PORT 2020
#define BUFSIZE 2048

struct fd_socket
{
    int fd;
    struct sockaddr_in server_addr;
};

#endif
