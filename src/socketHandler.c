#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "socketHandler.h"

int initServerSocket(unsigned short port)
{
    struct sockaddr_in socketAddr;
    int optval = 1;
    
    int serverFd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if(serverFd == -1){
        return -1;
    }

	setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR,
		   (const void *)&optval, sizeof(int));

    bzero((char *)&socketAddr, sizeof(socketAddr));
    socketAddr.sin_family = AF_INET;
    socketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    socketAddr.sin_port = htons((unsigned short)port);

    if (bind(serverFd, (struct sockaddr *) &socketAddr, sizeof(socketAddr)) < 0) {
        perror("server: bind()");
        close(serverFd);
        return -1;
    }
    return serverFd;
}

int readFrame(int orderFd, char *clientMsg, struct sockaddr_in *clientAddr)
{
    unsigned int length =  sizeof(struct sockaddr_in);
    if(recvfrom(orderFd, clientMsg, 1024, 0,(struct sockaddr*)clientAddr, &length) < 0){
        return -1;
    }
    return 0; 
}
