#include<unistd.h>
#include<string.h>

#include "Server.h"
#include "EventDispatcher.h"
#include "Connection.h"
#include "shmtx.h"

#include<iostream>
using namespace std;

Server::Server(uint32_t port,EventDispatcher* p)
:_isAccept(false)
,_stop(false)
,_port(port)
,_dispatcher(p)
,_acceptChan(new Channel()){
    bzero(&_serverAddr,sizeof(_serverAddr));
    initServer("",port);
}

Server::Server(uint32_t port)
:_isAccept(false)
,_stop(false)
,_port(port)
,_acceptChan(new Channel()){
    bzero(&_serverAddr,sizeof(_serverAddr));
    initServer("",port);
}

Server::Server(const std::string& ip,uint32_t port,EventDispatcher* p)
:_isAccept(false)
,_stop(false)
,_port(port)
,_dispatcher(p)
,_acceptChan(new Channel()){
    bzero(&_serverAddr,sizeof(_serverAddr));
    initServer(ip,port);
}

Server::~Server(){
    if(_acceptChan){
        delete _acceptChan;
        _acceptChan = nullptr;
    }
}

void 
Server::initServer(const string& ip,uint32 port){
    uint32 fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < 0){
        cout<<"create listenfd failed."<<endl;
        return;
    }
    
    int val =1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val))<0) {
        cout<<"set reuse failed"<<endl;
    }
    //cout<<"init acceptChan addr : "<<_acceptChan<<endl;
    _acceptChan->SetFd(fd);
    _acceptChan->SetAccept(std::bind(&Server::Accept,this));
    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_port = htons(port);
    if(ip.empty()){
        _serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }else{
        _serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        if(inet_pton(AF_INET, ip.c_str(), &_serverAddr.sin_addr.s_addr) <= 0){
            cout<<"ip addr invalid"<<endl;
            throw "ip addr invalid";
        }
    }

    if(bind(fd,(struct sockaddr*)&_serverAddr,sizeof(_serverAddr)) < 0){
        cout<<"bind in port "<<port<<" failed!"<<endl;
        throw "bind failed!!!";
    }
    if(listen(fd,1024) < 0) {
        cout<<"listen in port "<<_port<<" failed."<<endl;
        throw "listend failed!!!";
    }
    return;
}

void 
Server::Accept(){
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    uint32 connfd = accept(_acceptChan->Fd(),(struct sockaddr *)&cliaddr,&clilen);
    if(connfd <= 0){
        cout<<"accept socket error: errno "<<strerror(errno)<<" "<<errno<<endl;
        return;
    }

    //auto newConn = std::make_shared<Connection>(connfd,_dispatcher);
    shared_ptr<Connection> newConn(new Connection(connfd,_dispatcher));
    newConn->SetMsgCb(_msgcb);
    newConn->SetCloseCb(std::bind(&Server::CloseConn,this,_1));
    _dispatcher->AddEvent(newConn->Chan(),EventNew);
    _conns.insert({connfd,newConn});
    //cout<<"accept socket new fd: "<<connfd<<"  pid:"<<getpid()<<endl;

    DisableAccept();
}

bool 
Server::EnableAccept(uint32 timeout){
    if(!_shmLocker){return false;}
    if(!_shmLocker->LockWithTimeout(5)){
        return false;
    }
    _dispatcher->UpdateEvent(_acceptChan,EventNew);
    return true;
}

void 
Server::DisableAccept(){
    if(!_shmLocker) return;
    if(_isAccept){
        _dispatcher->UpdateEvent(_acceptChan,EventDel);
        _shmLocker->Unlock();
    } 
    _isAccept = false;
    return;
}

void
Server::CloseConn(ConnectionPtr conn){
    _conns.erase(conn->PeerFd());
}

void
Server::Start(){
    //add listenfd to epoll
    //_dispatcher->AddEvent(_acceptChan,EventNew);

    //not use timer to awake epoll_wait,just set a timeout value.
    //start io detection
    while(!_stop){
        _isAccept = EnableAccept(5);
        //cout<<"server run "<<_isAccept<<endl;
        _dispatcher->Run();
        DisableAccept();
    }
    return;
}