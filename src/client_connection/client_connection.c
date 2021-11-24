#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

#include "client_connection.h"

void *threadProcess(void * ptr) {
    char buffer_in[BUFFERSIZE];
    int sockfd = *((int *) ptr);
    int len;
    while ((len = read(sockfd, buffer_in, BUFFERSIZE)) != 0) {
        if (strncmp(buffer_in, "exit", 4) == 0) {
            break;
        }

        printf("receive %d chars\n", len);
        printf("%.*s\n", len, buffer_in);
    }
    close(sockfd);
    printf("client pthread ended, len=%d\n", len);
}

int open_connection() {
    int sockfd;

    struct sockaddr_in serverAddr;
    int port = 7799;
 
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET; 
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

    if (connect(sockfd, (struct sockaddr *) &serverAddr, sizeof (serverAddr)) != 0) {
        printf("Fail to connect to server\n");
        exit(-1);
    };

    return sockfd;
}

void init_connection(){
    int sockfd;
    int status = 0;
    char msg[100];
    pthread_t thread;

    sockfd = open_connection();

    strcpy(msg, "Hello from Xeon"); 
    printf("sending : %s\n", msg);
    write(sockfd, msg, strlen(msg));

    pthread_create(&thread, 0, threadProcess, &sockfd);
    pthread_detach(thread);

    do {
        fgets(msg, 100, stdin);
        //printf("sending : %s\n", msg);
        status = write(sockfd, msg, strlen(msg));
        //memset(msg,'\0',100);
    } while (status != -1);
}