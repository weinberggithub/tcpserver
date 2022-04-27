#include<sys/socket.h>

#include "Connection.h"
#include "Channel.h"

#include<iostream>
using namespace std;
Connection::Connection(uint32 fd,EventDispatcher* dispacther)
:_sockfd(fd)
{
    _chan = new Channel(fd,dispacther);
    //cout<<"read callback addr: "<<&Connection::Send<<"  this"<<this<<endl;
    _chan->SetReadCallback(std::bind(&Connection::handleRead,this));
    _chan->SetWriteCallback(std::bind(&Connection::handleWrite,this));
}

Connection::~Connection(){
    if(_chan){
        delete _chan;
        _chan == nullptr;
    }
}

void 
Connection::handleRead(){
    int err;
    int ret = _inbuf.readFd(_chan->Fd(),&err);
    if(ret == -1){
        std::cout<<"read data return -1."<<std::endl;
        close(_chan->Fd());
        return;
    }
    if(ret == 0){
        handleClose();
        return;
    }
    if(_msgcb){
        _msgcb(shared_from_this());
    }
}

void 
Connection::handleWrite(){
    while(1){
        int ret = send(_chan->Fd(),_outbuf.peek(),_outbuf.readableBytes(),0);
        // if(ret == -1){
        //     std::cout<<"send buf may full."<<std::endl;
        //     break;
        // }
        if(ret == _outbuf.readableBytes()){
            _chan->UpdateChannel(EventRead);
            break;
        }else{
            //_outbuf.retrieve(ret);
            std::cout<<"send buf may full."<<std::endl;
            break;
        }
    }
}

void 
Connection::handleClose(){
    //: "<<_chan->Fd()<<std::endl;
    _chan->UpdateChannel(EventDel);
    close(_sockfd);
    return;
}


void 
Connection::handleErr(){
    return;
}


//TODO buffer capacity smaller than data length.
void
Connection::Send(const std::string& data){
    if(data.empty() || _outbuf.internalCapacity() < data.length()) return;
    int ret = send(_chan->Fd(),data.data(),data.length(),0);
    if(ret == data.length()){
        _chan->UpdateChannel(EventRead);
        return;
    }
    uint32 remaining = ret == -1 ? data.length() : data.length() - ret;
    //assert(remaining != 0);
    if(remaining == 0){
        std::cout<<"fucking impossible or Im foolish"<<std::endl;
        return;
    }
    if(data.data() == _outbuf.peek()){
        _outbuf.retrieve(ret);
    }else{
        _outbuf.append(data.data() + ret,remaining);
    }
    _chan->UpdateChannel(EventWrite);
}

void 
Connection::Send(Buffer* buf){
    Send(buf->toString());
}
