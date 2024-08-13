

int initServerSocket(unsigned short port);

/// @brief read frame receives from the server
/// @param socketFd socketFile directory
/// @param clientMsg client data receive 
/// @param clientAddr client address received 
/// @return 0 no error, -1 if issue while reading
int readFrame(int socketFd, char *clientMsg, struct sockaddr_in *clientAddr);