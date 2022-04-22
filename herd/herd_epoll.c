#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>

#include<iostream>
#include<vector>
#include<map>
#include<functional>



using namespace std;
using event_callback=std::function<void(int)>;

using buffer=unsigned char;

const int gevent_size = 1024;
const int gbuff_size = 4096;

uint32_t slef_pid = 0;
uint32_t listen_fd = 0;

struct Event{
    int fd;
    int event;
    event_callback acceptcb;
    event_callback readcb;
    event_callback writecb;

    buffer rbuff[gbuff_size];
    unsigned int rlen;
    buffer wbuff[gbuff_size];
    unsigned int wlen;
};

struct reactor{
    int epfd;
    //TODO  listenfd in reactor is a bad idea
    int listenfd;
    map<int,Event> events;
};

reactor* reactor_init(int event_size){
    reactor *r = new reactor();
    r->epfd = epoll_create(1);
    return r;
}
reactor* R = nullptr;
reactor* get_instance(){
    if(R == nullptr){
        R = reactor_init(gevent_size);
    }
    return R;
}

const int event_none = 0;
const int event_accept = 1;
const int event_read = 2;
const int event_write = 3;

void reactor_set_events(int fd,event_callback cb,int event_type){
    reactor* r = get_instance();
    struct epoll_event ev = {0};
    if(event_type == event_accept){
        Event event;
        event.fd = fd;
        event.acceptcb = cb;
        event.event = event_none;
        r->events.insert({fd,event});
        ev.events = EPOLLIN;
    }else if(event_type == event_read){
        if(r->events.count(fd) <= 0){
            Event newConn = {0};
            newConn.fd = fd;
            newConn.event = event_none;
            r->events.insert({fd,newConn});
        }
        r->events[fd].readcb = cb;
        ev.events = EPOLLIN;
    }else if(event_type == event_write){
        r->events[fd].writecb = cb;
        ev.events = EPOLLOUT;
    }else{
        cout<<"no possible"<<endl;
    }

    ev.data.ptr = &r->events[fd];
    
    if(r->events[fd].event == event_none){
        epoll_ctl(r->epfd,EPOLL_CTL_ADD,fd,&ev);
    }else{
        epoll_ctl(r->epfd,EPOLL_CTL_MOD,fd,&ev);
    }
    r->events[fd].event = event_type;
}

void write_callback(int fd);
void read_callback(int fd);
void accept_callback(int listenfd){
    reactor* r = get_instance();
    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
    if(connfd < 0){
        cout<<"accept socket error: errno "<<strerror(errno)<<" "<<errno<<endl;
        return;
    }
    //cout<<"new connection"<<endl;
    cout<<"pid: "<<slef_pid<<"  fd: "<<connfd<<endl;
    reactor_set_events(connfd,read_callback,event_read);
}

void read_callback(int fd){
    reactor* r = get_instance();
    //ET
    //
    struct epoll_event ev = {0};
    int ret = recv(fd,r->events[fd].rbuff,gbuff_size,0);
    r->events[fd].rlen = ret;
    if(ret == 0){
        //close
        epoll_ctl(r->epfd,EPOLL_CTL_DEL,fd,&ev);
        r->events.erase(fd);
        close(fd);
    }else{
        //cout<<"recv from "<<inet_ntoa(cliaddr.sin_addr)<<" : "<<buff<<endl;
        memcpy(r->events[fd].wbuff,r->events[fd].rbuff,ret);
        r->events[fd].wlen = ret;
        reactor_set_events(fd,write_callback,event_write);
        //send(events[i].data.fd,buff,readn,0);
    }
}

void write_callback(int fd){
    reactor* r = get_instance();
    int buff_len = r->events[fd].wlen;
    int ret = send(fd,r->events[fd].wbuff,buff_len,0);
    if(ret < buff_len){
        reactor_set_events(fd,write_callback,event_write);
    }else{
        reactor_set_events(fd,read_callback,event_read);
    }
}

void event_loop(){
    reactor* r = get_instance();
    struct epoll_event events[gevent_size] = {0};
    while(1){
        int nready = epoll_wait(r->epfd,events,gevent_size,-1);
        if(nready == -1){
            cout<<"awoken with no eevnt pid: "<<slef_pid<<endl;
            continue;
        }

        for(int i = 0;i < nready;++i){
            Event *event = (Event*)events[i].data.ptr;
            if(event->fd == r->listenfd){
                event->acceptcb(event->fd);
            }else{
                if(events[i].events & EPOLLIN){
                    try{
                        event->readcb(event->fd);
                    }catch(...){
                        cout<<"------exception----- pid: "<<slef_pid<<"  fd: "<<event->fd<<endl;
                    }
                    
                }
                if(events[i].events & EPOLLOUT){
                    event->writecb(event->fd);
                }
            }
        }
    }
}

int init_server(int port){
    //reactor* r = get_instance();
    if(port <=0 || port >= 65535){
        cout<<"port invalid "<<port<<endl;
        return -1;
    }

    struct sockaddr_in serverAddr;
    bzero(&serverAddr,sizeof(serverAddr));

    listen_fd = socket(AF_INET,SOCK_STREAM,0);
    if(listen_fd < 0){
        cout<<"create listenfd failed."<<endl;
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listen_fd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) < 0){
        cout<<"bind in port "<<port<<" failed!"<<endl;
        return -1;
    }

    if(listen(listen_fd,1024) < 0) {
        cout<<"listen in port "<<port<<" failed."<<endl;
        return -1;
    }
    return listen_fd;
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
    int listenfd = init_server(port);
    if(listenfd <= 0){
        cout<<"init tcpserver failed, listenfd = "<<listenfd<<endl;
        return -1;
    }
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
    slef_pid = (uint32_t)getpid();
    reactor* r = get_instance();
    r->listenfd = listen_fd;
    reactor_set_events(listenfd,bind(&accept_callback,listenfd),event_accept);
    event_loop();
    return 0;
}