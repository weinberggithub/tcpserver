#include"Buffer.h"
#include<sys/socket.h>
#include <errno.h>

const static uint32 gMaxBufferCap = 4096;

ssize_t 
Buffer::readFd(int fd, int* savedErrno){
    //ET not impl.
    //LT
    //recv vs read,recv has a flag para
    //,readv is for iovec,read data from kernel into unconsequently block by call syscall once.
    int ret = recv(fd,&*_buff.begin(),internalCapacity(),0);
    if(ret < 0){
        *savedErrno = errno;
        return ret;
    }
    _contentLen += ret;
    return ret;
}