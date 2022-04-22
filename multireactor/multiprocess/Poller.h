#ifndef __POLLER_H__
#define __POLLER_H__


class Poller{
public:
    virtual void EventLoop(uint32 listenfd) = 0;
    virtual ~Poller() = default;

protected:
    int _epfd;
};

#endif //__POLLER_H__