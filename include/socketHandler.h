




int initServerSocket(unsigned short port);

int initClientSocket(uint32_t addr,unsigned short port);

int creatSocket();

struct sockaddr_in creatInfoAddr(unsigned short port, uint32_t* addr);

/// @brief read frame receives from the server
/// @param socketFd socketFile directory
/// @param clientMsg client data receive 
/// @param clientAddr client address received 
/// @return 0 no error, -1 if issue while reading
int readFrame(int socketFd, char *clientMsg, struct sockaddr_in *clientAddr);

int sendFrame(int socketFd, char* dataToSend, size_t length, uint16_t port, uint32_t addr);