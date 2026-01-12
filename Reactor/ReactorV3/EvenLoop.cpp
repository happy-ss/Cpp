#include "EvenLoop.hpp"

namespace lg{

EventLoop::EventLoop(Acceptor &acceptor)
:_epfd(createEpollFd()),
_acceptor(acceptor),
_isLooping(false),
_events(1000),
_conns(){
    addEpollReadEvent(_acceptor.fd());
}
int EventLoop::createEpollFd(){
    return epoll_create1(0);
}

void EventLoop::addEpollReadEvent(int fd){
    struct epoll_event ev;
    bzero(&ev,sizeof(ev));
    ev.data.fd=fd;
    ev.events=EPOLLIN;
    int ret=epoll_ctl(_epfd,EPOLL_CTL_ADD,fd,&ev);
    if(ret<0){
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }
}

void EventLoop::loop(){
    _isLooping=true;
    while(_isLooping){
        waitEpollFd();
    }
}
void EventLoop::unloop(){
    _isLooping=false;
}
void EventLoop::waitEpollFd(){
    int nready=epoll_wait(_epfd,_events.data(),_events.size(),5000);
    if(nready==-1&&errno==EINTR){
        return;
    }else if(nready==-1){
        perror("epoll_wait");
        exit(EXIT_FAILURE);
    }else if(nready==0){
        printf("epoll_wait timeout\n");
    }else{
        for(int i=0;i<nready;++i){
            int fd=_events[i].data.fd;
            if(fd==_acceptor.fd()){
                handleConnetcion();
            }else{
                handleMessage(fd);
            }
        }
    }
}

void EventLoop::handleConnetcion(){
    int netfd=_acceptor.accept();
    TcpConnectionPtr conn(new TcpConnection(netfd));
    conn->setAllCallbacks(_onConnction,
                            _onMessage,
                            _onClose);
    _conns.insert(std::make_pair(netfd,conn));
    addEpollReadEvent(netfd);
    conn->handleConnectionCallbacks();
}
void EventLoop::handleMessage(int fd){
    auto it=_conns.find(fd);
    if(it!=_conns.end()){
        bool flag=it->second->isClosed();
        if(flag){
            it->second->handleCloseCallbacks();
            delEpollReadEvent(fd);
            _conns.erase(fd);
        }else{
            it->second->handleMessageCallbacks();
        }
    }
}
void EventLoop::delEpollReadEvent(int fd){
    struct epoll_event ev;
    bzero(&ev,sizeof(ev));
    ev.data.fd=fd;
    ev.events=EPOLLIN;
    int ret=epoll_ctl(_epfd,EPOLL_CTL_DEL,fd,&ev);
    if(ret<0){
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }
}





}//end of namespace lg

