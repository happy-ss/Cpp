#include "Acceptor.hpp"

namespace lg{
Acceptor::Acceptor(unsigned short port,const char* ip)
:_sock(),
_inetAddr(port,ip)
{

}
void Acceptor::ready(){
    setReuseAddr(true);
    setReusePort(true);
    bind();
    listen();
}
void Acceptor::bind(){
    int ret=::bind(_sock.fd(),(const struct sockaddr*)_inetAddr.getSockaddrPtr(),sizeof(_inetAddr));
    if(ret<0){
        perror("bind");
    }
}
void Acceptor::listen(){
    int ret=::listen(_sock.fd(),10000);
    if(ret<0){
        perror("listen");
    }
}
void Acceptor::setReuseAddr(bool on){
    int tu=on;
    setsockopt(_sock.fd(),SOL_SOCKET,SO_REUSEADDR,&tu,sizeof(tu));
}
void Acceptor::setReusePort(bool on){
    int tu=on;
    setsockopt(_sock.fd(),SOL_SOCKET,SO_REUSEPORT,&tu,sizeof(tu));
}

int Acceptor::accept(){
    int netfd=::accept(_sock.fd(),nullptr,nullptr);
    if(netfd <0){
        perror("accept");
    }
    return netfd;
}
}
