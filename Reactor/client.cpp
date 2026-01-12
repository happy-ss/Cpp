#include <header.h>

int main(){
    int listenFd = socket(AF_INET,SOCK_STREAM,0);
    if(listenFd==-1){
        perror("socket");
        return EXIT_FAILURE;
    }
    struct sockaddr_in serverAddr;
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_port=htons(8080);
    serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    int ret = connect(listenFd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    if(ret==-1){
        perror("connect");
        return EXIT_FAILURE;
    }
    int epFd=epoll_create1(0);
    struct epoll_event ev;
    ev.data.fd=listenFd;
    ev.events=EPOLLIN;
    ret =epoll_ctl(epFd,EPOLL_CTL_ADD,listenFd,&ev);
    if(ret==-1){
        perror("epoll_ctl");
        return EXIT_FAILURE;
    }
    ev.data.fd=STDIN_FILENO;
    ev.events=EPOLLIN;
    ret =epoll_ctl(epFd,EPOLL_CTL_ADD,STDIN_FILENO,&ev);
    if(ret==-1){
        perror("epoll_ctl");
        return EXIT_FAILURE;
    }
    struct epoll_event events;
    bzero(&events,sizeof(events));
    while(1){
        int nready=epoll_wait(epFd,&events,1,-1);
        if(nready==-1){
            perror("epoll_wait");
        }else if(nready==0){
            perror("epoll timeout");
        }else{
            if(events.data.fd==listenFd){
                char buf[100]={0};
                int ret =recv(listenFd,buf,sizeof(buf),0);
                if(ret==-1){
                    perror("recv listenFd");
                }
                if(ret ==0){
                    printf("服务端断开连接\n");
                    return EXIT_FAILURE;
                }
                printf("recv:%s\n",buf);
            }else if(events.data.fd==STDIN_FILENO){
                char buf[100]={0};
                int ret =read(STDIN_FILENO,buf,sizeof(buf));
                if(ret==-1){
                    perror("read STDIN_FILENO");
                }
                send(listenFd,buf,ret,0);
            }
        }
    }
    
    close(listenFd);
    return 0;
}
 
