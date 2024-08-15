#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "socketHandler.h"

int initServerSocket(unsigned short port)
{
    int serverFd = creatSocket();
    struct sockaddr_in socketAddr = creatInfoAddr(port, NULL);

    if (bind(serverFd, (struct sockaddr *) &socketAddr, sizeof(socketAddr)) < 0) {
        perror("server: bind()");
        close(serverFd);
        return -1;
    }
    return serverFd;
}

int initClientSocket(uint32_t addr, unsigned short port)
{
    int clientFd = creatSocket();
    return clientFd;
}

int creatSocket()
{
    int optval = 1;
    int socketFd = socket(AF_INET, SOCK_DGRAM, 0); 
    if(socketFd == -1){
        return -1;
    }
    setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR,
		   (const void *)&optval, sizeof(int));
    return  socketFd;
}

struct sockaddr_in creatInfoAddr(unsigned short port, uint32_t *addr)
{
    struct sockaddr_in socketAddr;
    bzero((char *)&socketAddr, sizeof(socketAddr));
    
    socketAddr.sin_family = AF_INET;
    socketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(addr != NULL){
        socketAddr.sin_addr.s_addr = *addr;    
    }
    socketAddr.sin_port = htons((unsigned short)port);
    
    return socketAddr;
}
int setReceiveTimeOut(int socketFd){
    struct timeval timeout;
    timeout.tv_usec = 500*1000;
    if (setsockopt (socketFd, SOL_SOCKET, SO_RCVTIMEO, &timeout,sizeof timeout) < 0){
        perror("setsockopt failed\n");
        return -1;
    }
    return 0;
}

int readFrame(int orderFd, char *clientMsg, struct sockaddr_in *clientAddr)
{
    unsigned int length =  0;
    if(clientAddr != NULL)
        length = sizeof(struct sockaddr_in);
    
    if(recvfrom(orderFd, clientMsg, 1024, 0,(struct sockaddr*)clientAddr, &length) < 0){
        return -1;
    }
    return 0; 
}

int sendFrame(int socketFd, char* dataToSend, size_t length,uint16_t port, uint32_t addr)
{
    struct sockaddr_in servaddr = creatInfoAddr(port, &addr);
    if (sendto(socketFd, dataToSend, length, 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        perror("cannot send message");
        close(socketFd);
        return -1;
    }
    return 0;
}
