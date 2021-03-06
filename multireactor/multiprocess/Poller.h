#ifndef __POLLER_H__
#define __POLLER_H__
#include<map>
#include "Types.h"


class Channel;
class Poller{
public:
    virtual void EventLoop() = 0;
    virtual void UpdateChannel(Channel* chan,uint32 eType) = 0;
    virtual ~Poller();
    uint32  Fd(){ return _epfd; }
    void SetTimeout(uint timeout){ _timeout = timeout; }

protected:
    int _epfd;
    uint32 _timeout;
    std::map<uint32,Channel*> _chans;
};
#endif //__POLLER_H__