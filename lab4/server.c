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
            close(connfd);
            exit(0);
        }
    }

    close(sock_serv.fd); // TODO: Use this
}

void client_handler(int connfd) {
    time_t ticks;
    printf("Connection Established\n");
    char buffer[BUFSIZE];
    memset(&buffer, '\0', sizeof(buffer));
    read(connfd, buffer, sizeof(buffer)-1);

    if (strncmp("EHLO", buffer, 4) != 0) {
        printf("Initialization failed\n");
        sprintf(buffer, "%lu", strlen(INIT_FAIL_STR));
        
        write(connfd, buffer, strlen(buffer));
        write(connfd, INIT_FAIL_STR, strlen(INIT_FAIL_STR));
        return;
    };

    while(1) {
        memset(&buffer, '\0', sizeof(buffer));
        read(connfd, buffer, sizeof(buffer)-1);
        unsigned long len_incoming = atoi(buffer);
        printf("%s", buffer);

        while(len_incoming > 0)
        {
            unsigned long recv;
            memset(&buffer, '\0', sizeof(buffer));

            if (sizeof(buffer)-1 > len_incoming) {
                recv = len_incoming;
            } else {
                len_incoming = len_incoming - (sizeof(buffer)-1);
                recv = sizeof(buffer)-1;
            }

            int offset = read(connfd, buffer, recv);

            printf("%d\n", offset);
            printf("%s", buffer);

            if (offset == 0) {
                printf("\n");
                break;
            }

            if (offset < 0) {
                fprintf(stderr, "ERROR! OFFSET LESS THAN ONE");
                fprintf(stderr, "[...]Killing client");
                return;
            }
        }

        write(connfd, "OK", 2);
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

