#include<sys/epoll.h>

#include "Channel.h"
#include "EventDispatcher.h"

Channel::Channel()
:_fd(0){
}

#include<iostream>

using namespace std;
Channel::Channel(uint32 fd,EventDispatcher* dispatcher)
:_fd(fd)
,_dispatcher(dispatcher)
{
    cout<<this<<"   fd "<<fd<<endl;
}

void 
Channel::EventProcess(uint32 event){
    if ((event & EPOLLHUP) && !(event & EPOLLIN))
    {
        //TODO remote peer closed.
        close(_fd);
        return;
    }

    if (event & EPOLLERR)
    {
        //TODO error not handle yet.
        close(_fd);
        return;
    }
    if((event & (EPOLLIN)) && _acceptcb){
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

void
Channel::UpdateChannel(uint32 eType){
    _dispatcher->UpdateEvent(this,eType);
}