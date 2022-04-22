#ifndef __CHANNEL_H__
#define __CHANNEL_H__
#include<functional>

#include "Types.h"

// struct Event{
//     int fd;
//     int event;
//     event_callback acceptcb;
//     event_callback readcb;
//     event_callback writecb;

//     buffer rbuff[gbuff_size];
//     unsigned int rlen;
//     buffer wbuff[gbuff_size];
//     unsigned int wlen;
// };

class Channel{
    using eventCallback=std::function<void()>;
public:
    void SetReadCallback(eventCallback cb){ _readcb = std::move(cb); }
    void SetWriteCallback(eventCallback cb){ _writecb = std::move(cb); }
    void SetAccept(eventCallback cb){ _acceptcb = std::move(cb); }
    uint32 Fd(){return _fd;}
    void EventProcess(uint32 event);
private:
    int _fd;
    eventCallback _acceptcb;
    eventCallback _readcb;
    eventCallback _writecb;
};

#endif //__CHANNEL_H__