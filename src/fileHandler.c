#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "fileHandler.h"


int readFile(char* path, char** fileData){
    int fileFd = open(path,O_RDONLY);
    
    // checking for error
    if(fileFd == -1){
        if(errno == ENOENT){
            return ENOENT*-1;
        }
        return -1;
    }

    int sizeFile = getLengthFile(fileFd);
    if(sizeFile == -1)
        return -1;

    *fileData = malloc(sizeof(char)*sizeFile);

    read(fileFd,*fileData,sizeFile);
    
    return sizeFile;
}

int writeFile(int fileFd, char *data, size_t lengthFile)
{
    return write(fileFd,data,lengthFile);
}

int createFile(char *fileName)
{
    return open(fileName,O_APPEND | O_CREAT | O_WRONLY,0755);
}

int isFileExist(char* fileName)
{
    return access(fileName,F_OK);
}

long getLengthFile(int fileFd)
{
    off_t pos = lseek(fileFd,0,SEEK_CUR);

    if(pos == -1)
        return -1;

    off_t size = lseek(fileFd, 0, SEEK_END);
    lseek(fileFd, pos, SEEK_SET);
    return size;
}
