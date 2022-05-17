#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include "Buffer.h"

class Channel;
class EventDispatcher;
class Connection: public std::enable_shared_from_this<Connection>{
public:
    Connection(uint32 fd,EventDispatcher* dispacther);
    ~Connection();

    Channel* Chan(){ return _chan; }
    Buffer* InBUff(){ return &_inbuf; }
    
    void SetMsgCb(MessageCallback msgcb){ _msgcb = msgcb; }
    uint32 PeerFd(){ return _sockfd; }
    void SetCloseCb(CloseCallback cb){ _closecb = std::move(cb); }

    void Send(const std::string& data);
    void Send(Buffer* buf);
private:
    void handleRead();
    void handleWrite();
    void handleClose();
    void handleErr();
private:
    uint32          _sockfd;
    Channel*        _chan;
    Buffer          _inbuf;
    Buffer          _outbuf;
    MessageCallback _msgcb;
    CloseCallback   _closecb;
};

#endif //__CONNECTION_H__