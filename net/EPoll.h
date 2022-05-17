#ifndef __EPOLL_H__
#define __EPOLL_H__
#include<sys/epoll.h>
#include<vector>

#include "Types.h"
#include "Poller.h"

class Channel;
class EPoll : public Poller{
public:
    EPoll();
    ~EPoll();

    void EventLoop(uint32 listenfd) override;
    void UpdateChannel(Channel* chan,uint32 eType) override;

private:
    const uint32 _eventsSize;
    std::vector<epoll_event> _events;
};

#endif  //__EPOLL_H__