#ifndef __reactor_H__
#define __reactor_H__

#include<unistd.h>
#include <fcntl.h>

#include<functional>
#include<memory>

using namespace std;
using namespace std::placeholders;
using ReactorCallback=std::function<void(int)>;

//这个地方主要学习事件如何封装，包含了什么内容？？
//开源项目中是怎么封装的？？？
/*
整体思路？？？网络检测部分主要职责是什么？网络io部分主要职责是什么？事件怎么封装怎么绑定？不同fd处理流程是什么？

1. reactor就是非阻塞io+多路复用
2. 事件封装的内容(一般来说就是指reactor)，io多路复用、事件注册与删除(主要有io触发事件、读事件、写事件)、事件分发
*/

class Poller;
class EventDispatcher{
public:
    EventDispatcher();
    void SetAcceptCallBack(ReactorCallback cb){_acceptcb = cb;}
    void SetReadCallBack(ReactorCallback cb){_readcb = cb;}
    void SetWriteCallBack(ReactorCallback cb){_writecb = cb;}

    //void SetEvent();
    void Run();
    
private:
    bool                _stop;
    unique_ptr<Poller> _poller;
    ReactorCallback     _acceptcb;
    ReactorCallback     _readcb;
    ReactorCallback     _writecb;
};


#endif //__reactor_H__