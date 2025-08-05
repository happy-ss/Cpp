#include "Acceptor.hpp"


namespace lg{

Acceptor::Acceptor(unsigned short port,const char* ip)
:_addr(port,ip),
_sock(){
    
}

Acceptor::~Acceptor(){

}
void Acceptor::bind(){
    int ret= ::bind(_sock.fd(),(const struct sockaddr*)_addr.getSockaddrinPtr(),sizeof(_addr));
    if(ret<0){
        perror("bind");
        exit(EXIT_FAILURE);
    }
}
void Acceptor::listen(){
    int ret=::listen(_sock.fd(),2000);
    if(ret<0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
}
int Acceptor::accept(){
    int netfd=::accept(_sock.fd(),nullptr,nullptr);
    if(netfd<0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    return netfd;
}

void Acceptor::ready(){
    setReuseAddr();
    setReusePort();
    bind();
    listen();
}
void Acceptor::setReuseAddr(){
    int on=1;
    int ret=setsockopt(_sock.fd(),SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    if(ret<0){
        perror("setReuseAddr setsockopt");
        exit(EXIT_FAILURE);
    }
}
void Acceptor::setReusePort(){
    int on=1;
    int ret=setsockopt(_sock.fd(),SOL_SOCKET,SO_REUSEPORT,&on,sizeof(on));
    if(ret<0){
        perror("setReusePort setsockopt");
        exit(EXIT_FAILURE);
    }
}
int Acceptor::fd(){
    return _sock.fd();
}

}//end of namespace lg
