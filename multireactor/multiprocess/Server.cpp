#include<unistd.h>
#include<string.h>
#include "Server.h"

#include<iostream>
using namespace std;

Server::Server(uint32_t port,EventDispatcher* p)
:_port(port)
,_dispatcher(p){
    bzero(&_serverAddr,sizeof(_serverAddr));
    initServer("",port);
}

Server::Server(const std::string& ip,uint32_t port,EventDispatcher* p)
:_port(port)
,_dispatcher(p){
    bzero(&_serverAddr,sizeof(_serverAddr));
    initServer(ip,port);
}

void 
Server::initServer(const string& ip,uint32 port){
    _listenFd = socket(AF_INET,SOCK_STREAM,0);
    if(_listenFd < 0){
        cout<<"create listenfd failed."<<endl;
        return;
    }

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

    if(bind(_listenFd,(struct sockaddr*)&_serverAddr,sizeof(_serverAddr)) < 0){
        cout<<"bind in port "<<port<<" failed!"<<endl;
        return;
    }
    if(listen(_listenFd,1024) < 0) {
        cout<<"listen in port "<<_port<<" failed."<<endl;
        return;
    }
    return;
}

void
Server::NewConn(){
    return;
}