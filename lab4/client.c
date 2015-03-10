#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "mytelnet.h"

struct fd_socket open_connection(char *, int);
void loop(struct fd_socket *conn);

int main(int argc, char * argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <host of server> {port}\n", argv[0]);
        exit(EINVAL);
    }

    int portno = SERV_PORT;
    if (argc > 2)
    {
        portno = atoi(argv[2]);
    }

    struct fd_socket conn = open_connection(argv[1], portno);
    while(1)
        loop(&conn);

    exit(0);
}

void loop(struct fd_socket *conn) {
    char buffer[BUFSIZE];
    char auxbuf[BUFSIZE];

    memset(buffer, '\0', sizeof(buffer));
    memset(auxbuf, '\0', sizeof(auxbuf));
    printf("[[> ");
    fgets(buffer, (BUFSIZE-1), stdin);

    sprintf(auxbuf, "%lu\n", strlen(buffer));
    write(conn->fd, auxbuf, strlen(auxbuf));
    read(conn->fd, auxbuf, strlen(auxbuf));
    write(conn->fd, buffer, strlen(buffer));

    if (!strcmp(buffer, "exit\n") || !strncmp(buffer, "exit ", 5))
    {
        printf("Shutting down...\n");
        exit(0);
    }

    while(1) {
        memset(auxbuf, '\0', sizeof(auxbuf));
        read(conn->fd, auxbuf, sizeof(auxbuf)-1); // Length of incoming
        write(conn->fd, auxbuf, strlen(auxbuf));
        unsigned long len_incoming = atoi(auxbuf);

        if (len_incoming == 0)
            break;

        while(len_incoming > 0)
        {
            unsigned long recv;
            memset(buffer, '\0', sizeof(buffer));

            if (sizeof(buffer)-1 > len_incoming) {
                recv = len_incoming;
                len_incoming = 0;
            } else {
                len_incoming = len_incoming - (sizeof(buffer)-1);
                recv = sizeof(buffer)-1;
            }

            printf("%lu\n", recv);
            int offset = read(conn->fd, buffer, recv);
            printf("%s", buffer);

            if (offset == 0)
                break;
        }
    }
}

struct fd_socket open_connection(char * ipaddr, int port) {
    struct fd_socket rv;
    rv.fd = 0;
    memset(&rv.server_addr, '\0', sizeof(rv.server_addr));

    rv.fd = socket(AF_INET, SOCK_STREAM, 0);

    if (rv.fd < 0)
    {
        fprintf(stderr, "\n Error: Could not initialize socket\n");
        exit(rv.fd);
    }

    rv.server_addr.sin_family = AF_INET;
    rv.server_addr.sin_port = htons(port);

    if(inet_pton(AF_INET, ipaddr, &rv.server_addr.sin_addr) <= 0)
    {
        fprintf(stderr, "\n Error: Could not parse IP %s\n", ipaddr); 
        exit(-1);
    }

    if (connect(rv.fd, 
        (struct sockaddr *) &rv.server_addr, sizeof(rv.server_addr)) < 0)
    {
        fprintf(stderr, "\n Error: could not connect to server\n");
        exit(-1);
    }

    return rv;
}

