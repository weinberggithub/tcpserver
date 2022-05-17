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
    //cout<<this<<"   fd "<<fd<<endl;
}

void 
Channel::EventProcess(uint32 event){
    if ((event & EPOLLHUP) && !(event & EPOLLIN))
    {
        //TODO remote peer closed. close callback
        //close(_fd);
        UpdateChannel(EventClose);
        if(_closecb) _closecb();
        return;
    }

    if (event & EPOLLERR)
    {
        //TODO error not handle yet. error callback
        if(_errorcb) _errorcb();
        return;
    }
    if((event & (EPOLLIN)) && _acceptcb){
        return _acceptcb();
    }
    if (event & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        if (_readcb) {
            //TODO  为什么这个地方没有读取呢？偶现不读取是怎么回事情？
            //cout<<"fd:  "<<_fd<<"  read pid: "<<getpid()<<endl;
            _readcb();
        }
        
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