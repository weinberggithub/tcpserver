#include"Buffer.h"
#include<sys/socket.h>
#include <errno.h>
#include<string.h>
#include<iostream>

const static uint32 gMaxBufferCap = 4096;

Buffer::Buffer(){
    _buff.resize(gMaxBufferCap);
}

Buffer::~Buffer(){}

ssize_t 
Buffer::readFd(int fd, int* savedErrno){
    //ET not impl.
    //LT
    //recv vs read,recv has a flag para
    //,readv is for iovec,read data from kernel into unconsequently block by call syscall once.
    int ret = recv(fd,&*_buff.begin(),internalCapacity(),0);
    //std::cout<<"buffer read ret:"<<ret<<"  remain len:"<<internalCapacity()<<std::endl;
    if(ret < 0){
        //std::cout<<"why the fucking error msg not print out????"<<std::endl;
        int optval;
        socklen_t optlen = static_cast<socklen_t>(sizeof optval);

        ::getsockopt(fd, SOL_SOCKET, SO_ERROR, &optval, &optlen);
        char errnobuf[512];
        bzero(errnobuf,sizeof errnobuf);
        printf("read error fd:%d  err:%d\n",fd,errno);
        strerror_r(errno, errnobuf, sizeof errnobuf);
        printf("read error :  %s\n",errnobuf);
        return ret;
    }
    _contentLen += ret;
    return ret;
}