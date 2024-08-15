

int readFile(char* path, char** fileData);
int writeFile(int fileFd,char* data, size_t lengthFile);
int createFile(char* fileName);
int isFileExist(char* fileName);
long getLengthFile(int fileFd);