#define _XOPEN_SOURCE 600
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "mytelnet.h"

#define NUM_CONNECTIONS 10
#define INIT_FAIL_STR "Client should send EHLO as first message"

struct fd_socket init_server(int port);
void client_handler(int);

int main(int argc, char * argv[])
{
    int port = SERV_PORT;

    if (argc > 1)
    {
        port = atoi(argv[1]);
    }

    struct fd_socket sock_serv = init_server(port);
    int connfd;

    while (1)
    {
        connfd = accept(sock_serv.fd, (struct sockaddr*) NULL, NULL);

        pid_t chld = fork(); 
        if (chld < 0)
        {
            fprintf(stderr, "Forking error");
        }
        else if (chld == 0)
        { 
            client_handler(connfd);
            printf("Connection terminated...\n");
            close(connfd);
            exit(0);
        }
    }

    close(sock_serv.fd); // TODO: Use this
}

void client_handler(int connfd) {
    printf("Connection Established\n");
    char buffer[BUFSIZE];
    char auxbuf[BUFSIZE];
    unsigned long recv;
    unsigned long len_incoming;

    memset(buffer, '\0', sizeof(buffer));
    memset(auxbuf, '\0', sizeof(auxbuf));

    while(1) {
        memset(buffer, '\0', sizeof(buffer));
        fgets(buffer, sizeof(buffer)-1, fdopen(connfd, "r"));
        len_incoming = atoi(buffer);
        write(connfd, buffer, strlen(buffer));

        if (sizeof(buffer)-1 > len_incoming) {
            recv = len_incoming;
            len_incoming = 0;
        } else {
            fprintf(stderr, "ERROR! Command too long");
            fprintf(stderr, "[...]Killing client");
            return;
        }

        memset(buffer, '\0', sizeof(buffer));
        fgets(buffer, recv, fdopen(connfd, "r")); // Get command
        printf("Recvd: %s\n", buffer);
        
        if (!strcmp(buffer, "exit\n") || !strncmp(buffer, "exit ", 5))
            return;

        if (!strncmp(buffer, "cd ", 3))
        {
            char * directory = buffer+3;
            char * c = strchr(directory, '\n');

            if (c != NULL)
                *c = '\0';

            chdir(directory);
            memset(buffer, '\0', sizeof(buffer));
            getcwd(buffer, sizeof(buffer));
            memset(auxbuf, '\0', sizeof(auxbuf));
            sprintf(auxbuf, "%lu\n", strlen(buffer)+1);
            sprintf(buffer, "%s\n", buffer);
            write(connfd, auxbuf, strlen(auxbuf));
            read(connfd, auxbuf, strlen(auxbuf)); // Client confirms len
            write(connfd, buffer, strlen(buffer));
            write(connfd, "0\n", 2);
            read(connfd, auxbuf, 2); // Client confirms len
        } else {
            FILE * cmd = popen(buffer, "r");
            while (1)
            {
                memset(buffer, '\0', sizeof(buffer));
                fgets(buffer, sizeof(buffer)-1, cmd);

                memset(auxbuf, '\0', sizeof(auxbuf));
                sprintf(auxbuf, "%lu\n", strlen(buffer));
                write(connfd, auxbuf, strlen(auxbuf));
                read(connfd, auxbuf, strlen(auxbuf)); // Client confirms len
                if (strlen(buffer) > 0)
                    write(connfd, buffer, strlen(buffer));
                else
                    break;
            }
            pclose(cmd);
        }
    }
}

struct fd_socket init_server(int port)
{
    struct fd_socket rv;
    rv.fd = 0;
    memset(&rv.server_addr, '\0', sizeof(rv.server_addr));
    
    errno = 0;
    rv.fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (errno != 0) {
        fprintf(stderr, "Could not initialize socket\n");
        exit(-1);
    }

    rv.server_addr.sin_family = AF_INET;
    rv.server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    rv.server_addr.sin_port = htons(port);

    errno = 0;

    bind(
        rv.fd,
        (struct sockaddr*) &rv.server_addr,
        sizeof(rv.server_addr)
    );
    
    if (errno != 0) {
        fprintf(stderr, "Could not bind socket to port\n");
        exit(-1);
    }

    listen(rv.fd, NUM_CONNECTIONS);

    if (errno != 0) {
        fprintf(stderr, "Could not listen on port with %d connections\n",
            NUM_CONNECTIONS);
        exit(-1);
    }

    printf("Listening on port %d\n", port);

    return rv;
}

