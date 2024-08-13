#include <stdio.h>
#include "tftpProtocol.h"

int main(int argc, char** argv){
    int socketOrderFd = initTftpServer();    
    puts("socket created");
    while(1){
        readConnectionRequest(socketOrderFd);
    }
    closeTftpServer(socketOrderFd);
}