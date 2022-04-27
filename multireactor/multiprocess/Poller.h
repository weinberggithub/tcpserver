#ifndef __POLLER_H__
#define __POLLER_H__
#include<map>
#include "Types.h"


class Channel;
class Poller{
public:
    virtual void EventLoop(uint32 listenfd) = 0;
    virtual void UpdateChannel(Channel* chan,uint32 eType) = 0;
    virtual ~Poller();
    uint32  Fd(){ return _epfd; }

protected:
    int _epfd;
    std::map<uint32,Channel*> _chans;
};
#endif //__POLLER_H__