#ifndef __CHANNEL_H__
#define __CHANNEL_H__
#include "Types.h"

#include<iostream>
using namespace std;
class EventDispatcher;
class Channel{
public:
    Channel(uint32 fd,EventDispatcher* dispatcher);
    Channel();
    void SetReadCallback(EventCallback cb){ _readcb = std::move(cb);}
    void SetWriteCallback(EventCallback cb){ _writecb = std::move(cb); }
    void SetAccept(EventCallback cb){ _acceptcb = std::move(cb); }
    uint32 Fd(){return _fd;}
    void SetFd(uint32 fd){ _fd = fd; }
    void EventProcess(uint32 event);

    void UpdateChannel(uint32 eType);
private:
    uint32 _fd;
    EventCallback _acceptcb;
    EventCallback _readcb;
    EventCallback _writecb;
    EventDispatcher* _dispatcher;
};

#endif //__CHANNEL_H__