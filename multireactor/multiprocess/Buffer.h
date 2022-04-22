#ifndef __BUFFER_H__
#define __BUFFER_H__

#include<string.h>

#include<vector>
#include<string>

#include "Types.h"

class Buffer{
public:
    void swap(Buffer& rhs)
    {
        _buff.swap(rhs._buff);
    }

    size_t readableBytes() const
    { return _contentLen; }

    const char* peek() const
    { return _contentLen > 0 ? &_buff[0] : nullptr; }

    const char* findCRLF() const
    {
        return nullptr;
    }

    const char* findCRLF(const char* start) const
    {
        return nullptr;
    }

    const char* findEOL() const
    {
        return nullptr;
    }

    const char* findEOL(const char* start) const
    {
        return nullptr;
    }

    std::string toString() const
    {
        return _contentLen > 0 ? std::string(&_buff[0]) : std::string();
    }

    void append(const std::string& str)
    {
        if(str.size() > internalCapacity() - _contentLen){
            return;
        }
        memcpy(&*_buff.begin() + _contentLen,str.data(),str.size());
        _contentLen += str.size();
    }

    size_t internalCapacity() const
    {
        return _buff.capacity() - _contentLen;
    }
    
    ssize_t readFd(int fd, int* savedErrno);

 private:
    std::vector<char> _buff;
    uint32 _contentLen;
};

#endif //__BUFFER_H__