#include "Types.h"
#include "EventDispatcher.h"
#include "Channel.h"
#include "EPoll.h"


#include<iostream>
EventDispatcher::EventDispatcher()
:_stop(false)
,_poller(new EPoll())
{
    std::cout<<"epoll fd : "<<_poller->Fd()<<std::endl;
}

void
EventDispatcher::Run(){
    while(!_stop){
        _poller->EventLoop(0);
    }
}


void
EventDispatcher::AddEvent(Channel *chan,uint32 eType){
    _poller->UpdateChannel(chan,eType);
}

void
EventDispatcher::UpdateEvent(Channel *chan,uint32 eType){
    _poller->UpdateChannel(chan,eType);
}