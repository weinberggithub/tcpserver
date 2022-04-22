#include<sys/select.h>
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
    vector<int> fd;
    int maxConn = 0;
    fd.resize(FD_SETSIZE  + 1);
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
    int maxfd = listenfd;
    fd_set allset,rset;
    FD_ZERO(&allset); 
    FD_SET(listenfd, &allset);
    while(1){
        rset = allset;
        int nready = select(maxfd + 1,&rset,NULL,NULL,NULL);
        if(FD_ISSET(listenfd,&rset)){
            socklen_t clilen = sizeof(cliaddr);
            int connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);
            if(maxConn >= FD_SETSIZE){
                cout<<"too many connections."<<endl;
                continue;
            }
            for(int i = 0;i < FD_SETSIZE;++i){
                if(fd[i] <= 0) {
                    fd[i] = connfd;
                    break;
                }
            }
            maxConn++;
            FD_SET(connfd,&allset);
            if(connfd > maxfd) maxfd = connfd;
            if(--nready <= 0) continue;
        }
        for(int i = listenfd;i <= maxConn;++i){
            if(fd[i] <= 0) continue;
            if(FD_ISSET(fd[i],&rset)){
                int readn = recv(fd[i],buff,sizeof(buff),0);
                if(readn == 0) {
                    close(fd[i]);
                    FD_CLR(fd[i],&allset);
                    fd[i] = -1;
                }else{
                    cout<<"recv from "<<inet_ntoa(cliaddr.sin_addr)<<" : "<<buff<<endl;
                    send(fd[i],buff,readn,0);
                    if(--nready <= 0) break;
                }
            }
        }
    }

}