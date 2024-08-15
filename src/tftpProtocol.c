#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h> 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include "socketHandler.h"
#include "tftpProtocol.h"
#include "fileHandler.h"


int initTftpServerSocket()
{
    return initServerSocket(REQUEST_PORT);
}

int initTftpClientSocket(struct clientInformation clientInfo)
{
    int clientFd = initClientSocket(clientInfo.addr, clientInfo.port);
    setReceiveTimeOut(clientFd);
    return clientFd;
}

struct tftpFrameOrder readConnectionRequest(int socketFd, struct clientInformation* clientInfo)
{
    char data[FRAME_RRQ_WRQ];
    struct sockaddr_in clientAddr;

    if(readFrame(socketFd,data,&clientAddr) == -1){
        perror("cannot read socket");
        exit(-1);
    }
    clientInfo->addr = clientAddr.sin_addr.s_addr;
    clientInfo->port = htons(clientAddr.sin_port);
    
    return parseConnectionRequest(data);
}

struct tftpFrameOrder parseConnectionRequest(char *data)
{
    struct tftpFrameOrder orderStructur;
    memset(&orderStructur,0,sizeof(struct tftpFrameOrder));
    
    orderStructur.opcode = (data[0] << 8) + data[1]; // get opCode

    // get fileName
    strcpy(orderStructur.fileName, &data[OPTION_CODE_LENGTH]);
    
    // get mode
    int fileNameLength = strlen(orderStructur.fileName) + 1;
    strcpy(orderStructur.mode, &data[OPTION_CODE_LENGTH+fileNameLength]);

    return orderStructur;
}

int dataBlocksCount(int fileLength)
{
    int blocksCount = fileLength/DATA_BLOCK;
    blocksCount += fileLength%DATA_BLOCK == 0 ? 0 : 1;
    return blocksCount;
}

char* tftpGenerateBLocks(int* sizeBlock,char *rawData, int fileLength, uint16_t idBlock, int nbBlocks)
{
    int currentIndex = idBlock*DATA_BLOCK;
    *sizeBlock = DATA_BLOCK;
    if(idBlock+1 == nbBlocks){
        *sizeBlock=fileLength%DATA_BLOCK;
    }
    char* block = malloc(sizeof(char)*(*sizeBlock));

    memcpy(block,&rawData[currentIndex],*sizeBlock);
    return block;
}

void tftpSendDataFrame(int clientFd, struct clientInformation clientInfo,char* block, int sizeBlock, uint16_t idBlock)
{
    // create Fram
    char dataFram[sizeBlock+4];
    memset(dataFram,0,sizeof(dataFram));
    dataFram[0]=0;dataFram[1]=3;dataFram[2]=(idBlock>>8); dataFram[3]= 0xff&idBlock;
    memcpy(&dataFram[4],block,sizeBlock);
    sendFrame(clientFd,dataFram,sizeBlock+4,clientInfo.port, clientInfo.addr);
}

int tftpWaitForACK(int clientFd,struct clientInformation clientInfo,uint16_t idBlock)
{
    char ACKFrame[4];
    if(readFrame(clientFd,ACKFrame,NULL) == -1){
        sendErrorPacket(clientFd,clientInfo,TIME_OUT,"Time out");
        return -1;
    }
    uint16_t opCode = (ACKFrame[0]<<8) + ACKFrame[1];
    uint16_t ACKIdBLock = (ACKFrame[2]<<8) + ACKFrame[3];
    
    if(opCode != ACK){
        sendErrorPacket(clientFd,clientInfo,ILLEGAL_OPERATION,"this is not an ACK packet");
        return -1;
    }

    if(ACKIdBLock != idBlock){
        sendErrorPacket(clientFd,clientInfo,UNKNOWN_TRANSFER_ID,"Invalid Id blocks");
        return -1;
    }
    return 0;
}

int tftpStartRRQProcess(int clientFd, struct clientInformation clientInfo, struct tftpFrameOrder frameOrder)
{
    int pathLength = strlen(frameOrder.fileName) + strlen("path/");

    char path[pathLength];    
    strcpy(path, "path/");
    strcat(path, frameOrder.fileName);
    
    if(strcmp(frameOrder.mode,"octet") != 0){
        sendErrorPacket(clientFd, clientInfo,NOT_IMPLEMENTED_YET, "the Mode selected is not implementend now");
        return -1;
    }

    char* rawData = NULL;
    int fileLength = readFile(path, &rawData);
    int nbBlocks = dataBlocksCount(fileLength);

    for(uint16_t i = 0; i < nbBlocks;i++){
        int sizeBlock = 0;
        char* block = tftpGenerateBLocks(&sizeBlock,rawData, fileLength,i, nbBlocks);
        tftpSendDataFrame(clientFd, clientInfo ,block,sizeBlock,i);
        free(block);
        if(tftpWaitForACK(clientFd,clientInfo,i) == -1){
            puts("exit now");
            break;
        }
    }

    free(rawData);
    return 0;
}

int tftpStartWRQProcess(int clientFd, struct clientInformation clientInfo, struct tftpFrameOrder frameOrder)
{
    return 0;
}

void  sendErrorPacket(int clientFd, struct clientInformation clientInfo,ErrorCodes idError, char* errorMsg)
{
    // building frame
    int packetLength = strlen(errorMsg)+5;

    char msg[packetLength];
    memset(msg,0,sizeof(msg));

    //buildingFrame
    msg[0] = 0; msg[1] = ERROR;msg[2] = 0;msg[3] = idError;
    memcpy(&msg[4],errorMsg,strlen(errorMsg));
    
    sendFrame(clientFd, msg, packetLength, clientInfo.port, clientInfo.addr);
}

void stopTftpSocket(int socketFd)
{
    close(socketFd);   
}
