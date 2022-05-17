#ifndef __TYPES_H__
#define __TYPES_H__
#include<unistd.h>
#include<functional>
#include<memory>

using uint32 = uint32_t;
using namespace std::placeholders;
using EventCallback = std::function<void()>;

class Connection;
using ConnectionPtr = std::shared_ptr<Connection>;
using MessageCallback = std::function<void (const ConnectionPtr&)>;
using CloseCallback = std::function<void (const ConnectionPtr&)>;

static const uint32 EventNew    = 0;
static const uint32 EventRead   = 1;
static const uint32 EventWrite  = 2;
static const uint32 EventDel    = 3;
static const uint32 EventClose  = 4;
#endif //__TYPES_H__