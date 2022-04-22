#include<iostream>

#include "Server.h"
#include "EventDispatcher.h"

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
    EventDispatcher dispatcher;
    Server s(port,&dispatcher);
    //start fork，one process per loop
    int childs[5]  = {0};
    for(int i = 0;i < 5;++i){
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
    // reactor_set_events(listenfd,bind(&accept_callback,_1),event_accept);
    // event_loop();
    return 0;
}