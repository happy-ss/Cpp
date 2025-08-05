#include "EventLoop.hpp"

namespace lg{

EventLoop::EventLoop(Acceptor& acceptor)
:_epfd(creatEpollFd()),
_acceptor(acceptor),
_isLoop(false),
_eveArr(1000){
    addEpollReadEvent(_acceptor.fd());
}
EventLoop::~EventLoop(){
    close(_epfd);
}
void EventLoop::loop(){
    _isLoop=true;
    while(_isLoop){
        waitEpollFd();
    }
}
void EventLoop::unloop(){
    _isLoop=false;
}
void EventLoop::waitEpollFd(){
    int nready=epoll_wait(_epfd,_eveArr.data(),_eveArr.size(),5000);
    if(nready==-1&&errno==EINTR){
        return;
    }else if(nready==-1){
        perror("epoll_wait");
        exit(EXIT_FAILURE);
    }else if(nready==0){
        printf("epoll_wait timeout\n");
    }else{
        for(int i=0;i<nready;++i){
            int fd=_eveArr[i].data.fd;
            if(fd==_acceptor.fd()){
                handleNewConnection();
            }else{
                handleMessage(fd);
            }
        }

    }
}
void EventLoop::handleNewConnection(){
    int netfd=_acceptor.accept();
    addEpollReadEvent(netfd);
    TcpConnectionPtr conn(new TcpConnection(netfd));
    conn->setAllCallbacks(std::move(_onConnection),
                          std::move(_onMessage),
                          std::move(_onClose));
    _conns.insert(std::make_pair(netfd,conn));
    conn->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd){
    auto it=_conns.find(fd);
    if(it!=_conns.end()){
        bool isClosed=it->second->isClosed();
        if(isClosed){
            it->second->handleCloseCallback();
            delEpollReadEvent(fd);
            _conns.erase(fd);
        }else{
            it->second->handleMessageCallback();
        }
    }
}
int EventLoop::creatEpollFd(){
    int epfd=epoll_create1(0);
    if(epfd<0){
        perror("epoll_create1");
    }
    return epfd;
}
void EventLoop::addEpollReadEvent(int fd){
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.fd=fd;
    ev.events=EPOLLIN;
    epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&ev);
}
void EventLoop::delEpollReadEvent(int fd){
    struct epoll_event ev;
    memset(&ev,0,sizeof(ev));
    ev.data.fd=fd;
    ev.events=EPOLLIN;
    epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,&ev);

}



}//end of namespace lg
