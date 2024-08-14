#include <stdio.h>
#include "tftpProtocol.h"

int main(int argc, char** argv){
    int socketOrderFd = initTftpServerSocket();    
    puts("socket created");
    while(1){
        struct clientInformation clientInfo;

        struct tftpFrameOrder connectionFrame = readConnectionRequest(socketOrderFd, &clientInfo);
        int clientFd = initTftpClientSocket(clientInfo);
        switch(connectionFrame.opcode)
        {
        case(RRQ):
            puts("RRQ");
            break;
        case(WRQ):
            puts("WRQ");
            break;
        default:
            puts("request error");
            sendErrorPacket(clientFd,clientInfo,ILLEGAL_OPERATION,"Illegal TFTP operation.");
        }

    }
    stopTftpServer(socketOrderFd);
}