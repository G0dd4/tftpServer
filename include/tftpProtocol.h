#include <stdint.h>

#define DATA_BLOCK 512
#define REQUEST_PORT 1069
#define FRAME_RRQ_WRQ 512
#define FILE_NAME_LENGTH 100
#define MODE_LENGTH 100

typedef enum Operation{
    RRQ = 1,
    WRQ = 2,
    Data = 3,
    ACK= 4,
    Error= 5
} Operation;

struct clientInformation
{
    uint32_t addr;
    uint16_t port;
};

struct tftpFrameOrder
{
    Operation opcode;
    char fileName[FILE_NAME_LENGTH];
    char mode[MODE_LENGTH];
};

int initTftpServer();

struct tftpFrameOrder readConnectionRequest(int socketFd);

struct tftpFrameOrder parseConnectionRequest(char* data);



void closeTftpServer(int socketFd);