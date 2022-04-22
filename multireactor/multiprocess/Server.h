#ifndef __SERVER_H__
#define __SERVER_H__
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string>
#include<functional>

#include "Types.h"

class EventDispatcher;
class Server{
    using eventCallback=std::function<void()>;
public:
    Server(uint32 port,EventDispatcher* p);
    Server(const std::string& ip,uint32 port,EventDispatcher* p);

    void NewConn();

    void SetOnCall(eventCallback cb){ _msgcb = std::move(cb); }
    uint32 ListenFd(){return _listenFd;}
    //disable copy
    // Server(const Server&) = delete;
    // void operator=(const Server&) = delete;
private:
    void initServer(const string& ip,uint32 port);
private:
    int _listenFd;
    uint32 _port;
    EventDispatcher* _dispatcher;
    struct sockaddr_in _serverAddr;
    //eventCallback _acceptcb; server is also a acceptor.
    eventCallback _msgcb;
};

#endif //__SERVER_H__