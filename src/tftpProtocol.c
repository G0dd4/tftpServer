#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h> 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "socketHandler.h"
#include "tftpProtocol.h"


int initTftpServer()
{
    return initServerSocket(REQUEST_PORT);
}

struct tftpFrameOrder readConnectionRequest(int socketFd)
{
    char data[FRAME_RRQ_WRQ];
    struct sockaddr_in clientAddr;

    if(readFrame(socketFd,data,&clientAddr) == -1){
        perror("cannot read socket");
        exit(-1);
    }

    return parseConnectionRequest(data);
}

struct tftpFrameOrder parseConnectionRequest(char *data)
{
    struct tftpFrameOrder orderStructur;
    orderStructur.opcode = (data[0] << 8) + data[1]; // get opCode

    // get fileName
    int fileNameLength;
    int modeLength;

    for(fileNameLength = 0; fileNameLength < FILE_NAME_LENGTH && data[fileNameLength+2]!=0; fileNameLength++){
        orderStructur.fileName[fileNameLength] = data[fileNameLength+2];
    }
    orderStructur.fileName[fileNameLength] = '\0';
    
    // get mode
    for(modeLength = 0; modeLength < MODE_LENGTH && data[modeLength+3+fileNameLength]!=0; modeLength++){
        orderStructur.mode[modeLength] = data[modeLength+3+fileNameLength];
    }
    orderStructur.mode[modeLength] = '\0';

    return orderStructur;
}

void closeTftpServer(int socketFd)
{
    close(socketFd);   
}
