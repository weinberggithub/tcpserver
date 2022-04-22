#include<sys/epoll.h>

#include "Epoll.h"
#include "Channel.h"

EPoll::EPoll()
:_eventsSize(1024)
{
    _epfd = epoll_create1(EPOLL_CLOEXEC);
    if(_epfd <= 0){
        //cout<<""
        throw "epoll_create failed.";
    }
    _events.resize(_eventsSize);
}

EPoll::~EPoll() = default;

#include<iostream>
using namespace std;
void 
EPoll::EventLoop(uint32 listenfd) {
    // blocking
    int nready = epoll_wait(_epfd,&*_events.begin(),_eventsSize,-1);
    if(nready == -1){
        return;
    }

    for(int i = 0;i < nready;++i){
        //how's the event designed and wrapped?
        Channel *chan = (Channel*)_events[i].data.ptr;
        if(_chans.find(chan->Fd()) == _chans.end()){
            cout<<"event exception happened "<<chan->Fd()<<endl;
            continue;
        }
        chan->EventProcess(_events[i].events);
    }
}