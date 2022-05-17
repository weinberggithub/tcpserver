#include<sys/epoll.h>

#include "EPoll.h"
#include "Channel.h"
#include "Types.h"

//tmp use stream
#include<iostream>
using namespace std;

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

EPoll::~EPoll(){
    close(_epfd);
}

int pcnt = 0;
void 
EPoll::EventLoop(uint32 listenfd) {
    // blocking
    int nready = epoll_wait(_epfd,&*_events.begin(),_eventsSize,-1);
    if(nready == -1){
        //errors
        return;
    }

    for(int i = 0;i < nready;++i){
        //how's the event designed and wrapped?
        Channel *chan = (Channel*)_events[i].data.ptr;
        //cout<<"getpid() : "<<getpid()<<"  chan addr: "<<chan<<endl;
        if(!chan){
            cout<<"getpid() : "<<getpid()<<"  event exception happened "<<"   addr: "<<chan<<endl;
            continue;
        }
        //TODO fix exception!!!
        if(_chans.find(chan->Fd()) == _chans.end() && pcnt != 0){
            // printf("i=%d event %x  ",i,_events[i].events);
            cout<<"getpid() : "<<getpid()<<"  event exception happened "<<chan->Fd()<<"   addr: "<<chan<<endl;
            pcnt = 1;
            continue;
        }
        chan->EventProcess(_events[i].events);
    }
}

string toString(int type){
    switch(type){
        case EventNew: return "new";
        case EventRead: return "mod read";
        case EventWrite: return "mod write";
        case EventClose: return "del close";
        case EventDel: return "del delete";
        default: return "none!!";
    }
}

void
EPoll::UpdateChannel(Channel* chan,uint32 eType){
    struct epoll_event ev = {0};
    ev.data.ptr = chan;
    switch(eType){
        case EventNew:{
            //cout<<"chan addr:"<<chan<<"  epoll add new sockfd: "<<chan->Fd()<<"  pid: "<<getpid()<<endl;
            ev.events = EPOLLIN;
            int ret = epoll_ctl(_epfd,EPOLL_CTL_ADD,chan->Fd(),&ev);
            if(ret != -1){
                _chans.insert({chan->Fd(),chan});
            }
            break;
        }
        case EventRead:{
            ev.events = EPOLLIN;
            epoll_ctl(_epfd,EPOLL_CTL_MOD,chan->Fd(),&ev);
            break;
        }
        case EventWrite:{
            ev.events = EPOLLOUT;
            epoll_ctl(_epfd,EPOLL_CTL_MOD,chan->Fd(),&ev);
            break;
        }
        case EventClose:{
            //cout<<"epoll close a sockfd: "<<chan->Fd()<<endl;
            epoll_ctl(_epfd,EPOLL_CTL_DEL,chan->Fd(),&ev);
            auto n = _chans.erase(chan->Fd());
            if(n != 1){
                cout<<"EventClose channel erase failed, n= "<<n<<"  fd:"<<chan->Fd()<<"  pid: "<<getpid()<<endl;
            }
            break;
        }
        case EventDel:{
            //cout<<"epoll del a sockfd: "<<chan->Fd()<<"   pid:"<<getpid()<<endl;
            epoll_ctl(_epfd,EPOLL_CTL_DEL,chan->Fd(),&ev);
            auto n = _chans.erase(chan->Fd());
            if(n != 1){
                cout<<"EventDel channel erase failed, n= "<<n<<"  fd:"<<chan->Fd()<<"  pid: "<<getpid()<<endl;
            }
            break;
        }
        default:{
            //error type of event.
            break;
        }
    }
    //cout<<"chan addr:"<<chan<<"  epoll "<<toString(eType)<<" sockfd: "<<chan->Fd()<<"  pid: "<<getpid()<<endl;
}