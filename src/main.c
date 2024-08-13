#include <stdio.h>
#include "tftpProtocol.h"

int main(int argc, char** argv){
    int socketOrderFd = initTftpServer();    
    puts("socket created");
    while(1){
        struct tftpFrameOrder connectionFrame = readConnectionRequest(socketOrderFd);

        switch(connectionFrame.opcode)
        {
        case(RRQ):
            puts("RRQ");
            break;
        case(WRQ):
            puts("WRQ");
            break;
        default:
            puts("request error\n");
        }

    }
    closeTftpServer(socketOrderFd);
}