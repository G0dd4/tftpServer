
int creatSocket();
struct sockaddr_in creatInfoAddr(unsigned short port, uint32_t* addr);
int initServerSocket(unsigned short port);
int initClientSocket(uint32_t addr,unsigned short port);
int setReceiveTimeOut(int socketFd);

/// @brief read frame receives by server
/// @param socketFd socket file description
/// @param clientMsg client data receive 
/// @param clientAddr client address received 
/// @return 0 no error, -1 if issue while reading
int readFrame(int socketFd, char *clientMsg, struct sockaddr_in *clientAddr);

/// @brief send frame to host
/// @param socketFd socket file description
/// @param dataToSend data to send to host
/// @param length data length
/// @param port port communication
/// @param addr host address  
/// @return 0 no error, -1 if issue while sending
int sendFrame(int socketFd, char* dataToSend, size_t length, uint16_t port, uint32_t addr);