#include "Types.h"
#include "EventDispatcher.h"
#include "Channel.h"
#include "EPoll.h"


#include<iostream>
EventDispatcher::EventDispatcher()
:_poller(new EPoll())
{
    //std::cout<<"epoll fd : "<<_poller->Fd()<<std::endl;
    //set timeout for subprocesses awake to handle connections established.
    _poller->SetTimeout(500);
}

void
EventDispatcher::Run(){
    _poller->EventLoop();
}


void
EventDispatcher::AddEvent(Channel *chan,uint32 eType){
    _poller->UpdateChannel(chan,eType);
}

void
EventDispatcher::UpdateEvent(Channel *chan,uint32 eType){
    _poller->UpdateChannel(chan,eType);
}