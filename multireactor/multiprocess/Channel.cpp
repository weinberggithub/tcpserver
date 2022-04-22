#include<sys/epoll.h>

#include "Channel.h"

Channel::Channel()
:_fd(0){
}

void 
Channel::EventProcess(uint32 event){
    if ((event & EPOLLHUP) && !(event & EPOLLIN))
    {
        //TODO remote peer closed.
    }

    if (event & EPOLLERR)
    {
        //TODO error not handle yet.
    }
    if(event & (EPOLLIN) && _acceptcb){
        return _acceptcb();
    }
    if (event & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        if (_readcb) _readcb();
    }
    if (event & EPOLLOUT)
    {
        if (_writecb) _writecb();
    }
}