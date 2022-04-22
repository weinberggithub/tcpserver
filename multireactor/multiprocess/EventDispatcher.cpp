#include "Poller.h"
#include "EventDispatcher.h"

EventDispatcher::EventDispatcher()
:_stop(false)
{}

void
EventDispatcher::Run(){
    while(!_stop){
        _poller->EventLoop(0);
    }
}
