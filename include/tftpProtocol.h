#include <stdint.h>

#define DATA_BLOCK 512
#define HEADER 4
#define REQUEST_PORT 1069
#define FRAME_RRQ_WRQ 512
#define OPTION_CODE_LENGTH 2
#define FILE_NAME_LENGTH 100
#define MODE_LENGTH 100

typedef enum Operation{
    RRQ = 1,
    WRQ = 2,
    DATA = 3,
    ACK= 4,
    ERROR= 5
} Operation;

typedef enum ErrorCodes{
    UNDIFINED = 0,
    FILE_NOT_FOUND = 1,
    ACCESS_VIOLATION = 2,
    DISK_FULL = 3,
    ILLEGAL_OPERATION = 4,
    UNKNOWN_TRANSFER_ID = 5,
    FILE_ALREADY_EXISTS = 6,
    NO_SUCH_USER = 7,
    TIME_OUT = 100,
    NOT_IMPLEMENTED_YET = 200
} ErrorCodes;

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

int initTftpServerSocket();
int initTftpClientSocket(struct clientInformation clientInfo);

struct tftpFrameOrder readConnectionRequest(int socketFd, struct clientInformation* clientInfo);
struct tftpFrameOrder parseConnectionRequest(char* data);

int dataBlocksCount(int fileLength);
char* tftpGenerateBLocks(int* sizeBlock,char *rawData, int fileLength, uint16_t idBlock, int nbBlocks);
void tftpSendDataFrame(int clientFd, struct clientInformation clientInfo,char* block, int sizeBlock, uint16_t idBlock);

int tftpWaitForACK(int clientFd,struct clientInformation clientInfo,uint16_t idBlock);
void tftpSendACKFrame(int socketFd, struct clientInformation clientInfo, uint16_t idBlock);

int tftpStartRRQProcess(int clientFd, struct clientInformation clientInfo, struct tftpFrameOrder frameOrder);
int tftpStartWRQProcess(int clientFd, struct clientInformation clientInfo, struct tftpFrameOrder frameOrder);

void sendErrorPacket(int clientFd, struct clientInformation clientInfo,ErrorCodes idError, char* errorMsg);
void stopTftpSocket(int socketFd);