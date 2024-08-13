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
    int fileNameLength;
    int modeLength;
    struct tftpFrameOrder orderStructur;
    memset(&orderStructur,0,sizeof(struct tftpFrameOrder));
    
    orderStructur.opcode = (data[0] << 8) + data[1]; // get opCode

    // get fileName
    for(fileNameLength = 0; fileNameLength < FILE_NAME_LENGTH && data[fileNameLength+OPTION_CODE_LENGTH]!=0; fileNameLength++){
        orderStructur.fileName[fileNameLength] = data[fileNameLength+OPTION_CODE_LENGTH];
    }
    fileNameLength++; // add \0 to length
    
    // get mode
    for(modeLength = 0; modeLength < MODE_LENGTH && data[modeLength+OPTION_CODE_LENGTH+fileNameLength]!=0; modeLength++){
        orderStructur.mode[modeLength] = data[modeLength+OPTION_CODE_LENGTH+fileNameLength];
    }
    return orderStructur;
}

void closeTftpServer(int socketFd)
{
    close(socketFd);   
}
