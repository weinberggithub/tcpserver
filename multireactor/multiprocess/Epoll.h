#ifndef __EPOLL_H__
#define __EPOLL_H__
#include<vector>
#include<map>

#include "Types.h"
#include "Poller.h"

class Channel;
class EPoll : public Poller{
public:
    EPoll();
    ~EPoll();

    void EventLoop(uint32 listenfd) override;

private:
    const uint32 _eventsSize;
    std::vector<epoll_event> _events;
    std::map<uint32,Channel*> _chans;
};

#endif  //__EPOLL_H__