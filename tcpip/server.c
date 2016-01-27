// based on
// http://www.linuxhowtos.org/C_C++/socket.htm
// http://www.linuxhowtos.org/data/6/server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MIN_DATA_SIZE 10
#define MAX_DATA_SIZE 1000
#define DATA_SIZE_INCR 10

#define TRIES 200
// send a size of 0 to signal the end of tests
#define TCP_END_SIGNAL 0

void send_buffer(int len, int newsockfd);
void error(const char *msg);

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[2048];
    int buffLen = 2048;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    int i;
    int tmp_int; // used to send over the socket

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    // setup
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
       error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0)
             error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,
                (struct sockaddr *) &cli_addr,
                &clilen);
    if (newsockfd < 0)
         error("ERROR on accept");

    // send increasing-size buffers
    for(i = MIN_DATA_SIZE; i < MAX_DATA_SIZE; i += DATA_SIZE_INCR) {
        send_buffer(i, newsockfd);
    }
    tmp_int = TCP_END_SIGNAL;
    write(newsockfd, &tmp_int, 4);

    close(newsockfd);
    close(sockfd);

    return 0;
}

void send_buffer(int len, int newsockfd) {
    int i, n;
    char *buffer;
    int tmp_int;

    buffer = malloc(len);
    if(buffer == 0) error("ERROR could not malloc");
    bzero(buffer, len);

    buffer[0] = 'D';
    buffer[1] = 'E';
    buffer[2] = 'A';
    buffer[3] = 'D';
    buffer[len - 4] = 'B';
    buffer[len - 3] = 'E';
    buffer[len - 2] = 'E';
    buffer[len - 1] = 'F';

    // send len
    n = write(newsockfd, &len, 4);
    if (n < 0) error("ERROR writing to socket");

    // send tries
    tmp_int = TRIES;
    n = write(newsockfd, &tmp_int, 4);
    if (n < 0) error("ERROR writing to socket");

    // send data
    for(i = 0 ; i < TRIES; i++) {
       n = write(newsockfd, buffer, len);
       if (n < 0) error("ERROR writing to socket");
    }

    free(buffer);
}
