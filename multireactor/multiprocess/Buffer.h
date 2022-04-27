#ifndef __BUFFER_H__
#define __BUFFER_H__

#include<string.h>

#include<vector>
#include<string>

#include "Types.h"

class Buffer{
public:
    Buffer();
    virtual ~Buffer();
    void swap(Buffer& rhs)
    {
        _buff.swap(rhs._buff);
        _rIdx = rhs._rIdx;
        _contentLen = rhs._contentLen;
    }

    size_t readableBytes() const
    { return _contentLen - _rIdx; }

    const char* peek() const
    { return _contentLen - _rIdx > 0 ? &_buff[0] : nullptr; }

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
        return _contentLen - _rIdx > 0 ? std::string(&_buff[0]) : std::string();
    }

    void append(const std::string& str)
    {
        if(str.size() > internalCapacity() - _contentLen){
            return;
        }
        memcpy(&*_buff.begin() + _contentLen,str.data(),str.size());
        _contentLen += str.size();
    }
    void append(const void* data,uint32 len)
    {
        if(len > internalCapacity() - _contentLen){
            return;
        }
        memcpy(&*_buff.begin() + _contentLen,data,len);
        _contentLen += len;
    }

    size_t internalCapacity() const
    {
        return _buff.capacity() - _contentLen;
    }
    
    ssize_t readFd(int fd, int* savedErrno);

    void retrieve(uint32 len){
        if(len < _contentLen){
            _rIdx += len;
        }else{
            _rIdx = _contentLen = 0;
        }
    }
 private:
    std::vector<char> _buff;
    uint32 _contentLen;
    uint32 _rIdx;
};

#endif //__BUFFER_H__