#include<sys/epoll.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>

#include<iostream>
#include<vector>


using namespace std;
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
    int listenfd,connfd;
    struct sockaddr_in serverAddr,cliaddr;
    bzero(&serverAddr,sizeof(serverAddr));

    listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd < 0){
        cout<<"create listenfd failed."<<endl;
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listenfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) < 0){
        cout<<"bind in port "<<port<<" failed!"<<endl;
        return -1;
    }

    if(listen(listenfd,1024) < 0) {
        cout<<"listen in port "<<port<<" failed."<<endl;
        return -1;
    }

    char buff[1024 * 4] = {};

    int epfd = epoll_create(1);
    struct epoll_event events[1024] = {0};
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = listenfd;
    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,listenfd,&ev);
    if(ret != 0) {
        cout<<"epoll_ctl failed."<<endl;
        return -1;
    }


    while(1){
        int nready = epoll_wait(epfd,events,1024,-1);
        for(int i = 0;i < nready;++i){
            if(events[i].data.fd == listenfd){
                //new connection
                socklen_t clilen = sizeof(cliaddr);
                int connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
                if(connfd < 0){
                    cout<<"accept socket error: errno "<<strerror(errno)<<" "<<errno<<endl;
                    return 0;
                }
                ev.data.fd = connfd;
                ev.events = EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,connfd,&ev);
            }else if(events[i].events & EPOLLIN){
                //read events
                int readn = recv(events[i].data.fd,buff,sizeof(buff),0);
                if(readn == 0) {
                    ev.data.fd = events[i].data.fd;
                    ev.events = EPOLLIN;
                    epoll_ctl(epfd,EPOLL_CTL_DEL,events[i].data.fd,&ev);
                    close(events[i].data.fd);
                }else{
                    cout<<"recv from "<<inet_ntoa(cliaddr.sin_addr)<<" : "<<buff<<endl;
                    send(events[i].data.fd,buff,readn,0);
                }
            }else if(events[i].events & EPOLLOUT){

            }else{
                cout<<"no possible"<<endl;
                return -1;
            }
        }
    }
    return 0;
}