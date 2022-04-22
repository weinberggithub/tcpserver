#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */
#include <sys/wait.h>
#include <unistd.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>

#include<iostream>

using namespace std;

const int PROC_NUM = 2;

#define ERREXIT(msg) \
    perror(msg);     \
    exit(-1)

#define SUCEXIT(msg) \
    printf(msg);     \
    exit(0)

#define PARAMERROREXIT(argc, argv) \
    usage(argc, argv);           \
    exit(-1)

void set_reuseaddr(int lfd) {
    int enable = 1;
    if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1){
        ERREXIT("set reuseaddr failed!");
    }
}

void set_reuseport(int lfd) {
    int enable = 1;
    if (setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int)) == -1){
        ERREXIT("set reuseport failed!");
    }
}

void set_noblock(int cfd) {
    int flag = fcntl(cfd, F_GETFL, 0);
    if (flag == -1) {
        ERREXIT("set set_noblock failed, get flag error!");
    }

    flag |= O_NONBLOCK;
    if (fcntl(cfd, F_SETFL, flag) == -1) {
        ERREXIT("set set_noblock failed, set flag error!");
    }
}

void proc_rw_event(int fd) {
    //have data need to read
    set_noblock(fd);
    while (1) {
        char *pbuf = new char[1024];
        memset(pbuf, 0, 1024);

        int nread = read(fd, pbuf, 1024);
        if (nread <= 0) {
            if (nread == 0 || errno == EAGAIN) {
                //have proc all read data
                break;
            } else {
                ERREXIT("call read data failed!");
            }
        }
        printf("[%d] ""%s", getpid(), std::string(pbuf, nread).c_str());

        delete[] pbuf;
    }
    close(fd);
}

int main(int argc, char **argv) {
    if(argc < 2){
        cout<<"no port input!!!"<<endl;
        return 0;
    }
    int port = atoi(argv[1]);
    if(port <= 0){
        cout<<"port invalid."<<endl;
        return -1;
    }
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        ERREXIT("call socket failed!");
    }

    set_reuseaddr(lfd);

    set_reuseport(lfd);

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(lfd, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) == -1) {
        ERREXIT("call bind failed!");
    }

    listen(lfd, 1024);

    int pid = 0;
    for (int i = 0; i < 5; ++i) {
        pid = fork();
        if (pid < 0) {
            ERREXIT("call fork failed!");
        }

        if (pid == 0)
            break;
    }

    if (pid > 0) {
        int status;
        wait(&status);
        return 0;
    }

    while (1) {
        int connfd = accept(lfd, (struct sockaddr *) NULL, NULL);
        if (connfd == -1) {
            ERREXIT("call accept failed!");
        }

        proc_rw_event(connfd);
    }

    return 0;
}