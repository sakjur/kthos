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

struct fd_socket init_server();
void client_handler(int);

int main(void)
{
    struct fd_socket sock_serv = init_server();
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
    char buffer[BUFSIZE];
    memset(&buffer, '\0', sizeof(buffer));
    read(connfd, buffer, sizeof(buffer)-1);
    printf("%s\n", buffer);

    write(connfd, "hej", strlen("hej"));
}

struct fd_socket init_server()
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
    rv.server_addr.sin_port = htons(SERV_PORT);

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

    printf("Listening on port %d\n", SERV_PORT);

    return rv;
}

