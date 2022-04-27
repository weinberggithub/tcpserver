#include<iostream>

#include "Server.h"
#include "EventDispatcher.h"
#include "Connection.h"
using namespace std;


void OnMessage(const ConnectionPtr& conn){
    auto in = conn->InBUff();
    conn->Send(in);
}

int main(int argc,char** argv){
    if(argc != 2){
        cout<<"input a listen port please!"<<endl;
        return 0;
    }
    int port = atoi(argv[1]);
    if(port <= 0){
        cout<<"port invalid."<<endl;
        return -1;
    }
    
    Server s(port);
    s.SetOnMsg(std::bind(&OnMessage,_1));
    //start fork，one process per loop
    int childs[5]  = {0};
    for(int i = 0;i < 0;++i){
        int pid = fork();
        if(pid == -1){
            exit(-1);
        }else if(0 == pid){
            //ppid return in child.
            break;
        }else{
            childs[i] = pid;
        }
    }
    EventDispatcher dispatcher;
    s.SetDispatcher(&dispatcher);
    s.Start();
    return 0;
}