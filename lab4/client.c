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
    loop(&conn);

    /*while((counter = read(conn.fd, buffer, sizeof(buffer)-1)) > 0)
    {
        printf("%s:%d >", argv[1], portno);
        buffer[counter] = '\0';
        if (fputs(buffer, stdout) == EOF)
        {
            fprintf(stderr, "\n Error: fputs error \n");
        }
    }*/

    exit(0);
}

void loop(struct fd_socket *conn) {
    int counter = 0;
    char buffer[BUFSIZE];
    char auxbuf[BUFSIZE];

    write(conn->fd, "EHLO", 4);

    while(1) {
        memset(buffer, '\0', sizeof(buffer));
        memset(auxbuf, '\0', sizeof(auxbuf));
        printf("[[> ");
        fgets(buffer, (BUFSIZE-1), stdin);
        
        sprintf(auxbuf, "%lu", strlen(buffer));
        write(conn->fd, auxbuf, strlen(auxbuf));
        write(conn->fd, buffer, strlen(buffer));

        if (!strcmp(buffer, "exit\n") || !strncmp(buffer, "exit ", 5))
        {
            printf("Shutting down...\n");
            exit(0);
        }

        read(conn->fd, buffer, sizeof(buffer)-1);
        printf("%s\n", buffer);

        if (counter < 0)
        {
            fprintf(stderr, "Error: Negative read buffer\n");
            exit(-1);
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

