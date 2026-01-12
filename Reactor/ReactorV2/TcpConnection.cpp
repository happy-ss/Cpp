#include "TcpConnection.hpp"
#include <sstream>

namespace  lg{

TcpConnection::TcpConnection(int fd)
:_sock(fd),
_localAddr(getLocalAddr()),
_peerAddr(getPeerAddr()),
_socketIO(fd){

}

InetAddress TcpConnection::getLocalAddr(){
    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    socklen_t len =sizeof(addr);
    int ret=getsockname(_sock.fd(),(struct sockaddr*)&addr,&len);
    if(ret<0){
        perror("getsockname");
    }
    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr(){
    struct sockaddr_in addr;
    bzero(&addr,sizeof(addr));
    socklen_t len =sizeof(addr);
    int ret=getpeername(_sock.fd(),(struct sockaddr*)&addr,&len);
    if(ret<0){
        perror("getpeername");
    }
    return InetAddress(addr);
}

void TcpConnection::send(const string&msg){
    if(msg.size()>0){
        _socketIO.sendn(msg.c_str(),msg.size());
    }
}
string TcpConnection::recv(){
    char buf[65535]={0};
    int ret =_socketIO.readLine(buf,sizeof(buf));
    return string(buf,ret);

}
void TcpConnection::shutdown(){
    _sock.shutdownWrite();
}

string TcpConnection::toString(){
    std::ostringstream oss;
    oss<<"tcp"<<_peerAddr.ip()<<":"<<_peerAddr.port()<<
        "->"<<_localAddr.ip()<<":"<<_localAddr.port()<<std::endl;
    return oss.str();
}
bool TcpConnection::isClosed(){
    char buf[20]={0};
    return _socketIO.recvPeek(buf,sizeof(buf))==0;
}
//TcpConnection这里不能使用移动语义，以为TcpConnection对象不止一个
void TcpConnection::setAllCallbacks(const Callback &cb1,
                     const Callback &cb2,
                     const Callback &cb3){
    _onConnection=cb1;
    _onMessage=cb2;
    _onClose=cb3;
}
void TcpConnection::handleConnectionCallback(){
    if(_onConnection){
        _onConnection(shared_from_this());
    }
}
void TcpConnection::handleMessageCallback(){
    if(_onMessage){
        _onMessage(shared_from_this());
    }
}
void TcpConnection::handleCloseCallback(){
    if(_onClose){
        _onClose(shared_from_this());
    }
}


}
