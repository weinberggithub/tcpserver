#ifndef __SERVER_H__
#define __SERVER_H__
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string>
#include<functional>
#include<map>
#include<memory>

#include "Types.h"
#include "Channel.h"

class shmtx;
class Connection;
class EventDispatcher;
class Server{
public:
    Server(uint32 port);
    Server(uint32 port,EventDispatcher* p);
    Server(const std::string& ip,uint32 port,EventDispatcher* p);
    ~Server();


    void Accept();
    bool EnableAccept(uint32 timeout);
    void DisableAccept();
    void CloseConn(ConnectionPtr conn);
    void Start();

    void SetOnMsg(MessageCallback cb){ _msgcb = std::move(cb); }
    uint32 ListenFd(){return _listenFd;}
    void SetDispatcher(EventDispatcher* dispatcher){ _dispatcher = dispatcher; }
    void SetLocker(shmtx* locker){ _shmLocker = locker; }
    //disable copy
    // Server(const Server&) = delete;
    // void operator=(const Server&) = delete;
private:
    void initServer(const std::string& ip,uint32 port);
private:
    bool _isAccept;
    bool _stop;
    int _listenFd;
    uint32 _port;
    EventDispatcher* _dispatcher;
    struct sockaddr_in _serverAddr;
    //eventCallback _acceptcb; server is also a acceptor.
    MessageCallback _msgcb;
    Channel* _acceptChan;
    shmtx*   _shmLocker;
    std::map<uint32,std::shared_ptr<Connection>> _conns;
};

#endif //__SERVER_H__