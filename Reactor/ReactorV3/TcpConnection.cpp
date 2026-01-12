#include "TcpConnection.hpp"
#include <sstream>
namespace lg{

TcpConnection::TcpConnection(int fd)
:_sock(fd),
_sockIO(fd),
_localAddr(getLocalAddr()),
_peerAddr(getPeerAddr())
{
       
}
InetAddress TcpConnection::getLocalAddr(){
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    socklen_t len=sizeof(addr);
    int ret= getsockname(_sock.fd(),(struct sockaddr*)&addr,&len);
    if(ret<0){
        perror("getsockname");
    }
    return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr(){
    struct sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    socklen_t len=sizeof(addr);
    int ret= getpeername(_sock.fd(),(struct sockaddr*)&addr,&len);
    if(ret<0){
        perror("getpeername");
    }
    return InetAddress(addr);
}
//adasdas
string TcpConnection::toString(){
    std::ostringstream oss;
    oss<<_peerAddr.ip()<<":"<<_peerAddr.port()<<"-->"
        <<_localAddr.ip()<<":"<<_localAddr.port();
    return oss.str();
}
string TcpConnection::recv(){
    char buf[1024]={0};
    int ret=_sockIO.recvLine(buf,sizeof(buf));
    return string(buf,ret);
}
void TcpConnection::send(const string &msg){
    _sockIO.sendn(msg.c_str(),msg.size());
}
bool TcpConnection::isClosed(){
    char buf[1024]={0};
    return _sockIO.recvPeek(buf,sizeof(buf))==0;
}
void TcpConnection::shutdown(){
    _sock.shutdownWrite();
}
void TcpConnection::handleConnectionCallbacks(){
    if(_onConnction){
        _onConnction(shared_from_this());
    }
}
void TcpConnection::handleMessageCallbacks(){
    if(_onConnction){
        _onMessage(shared_from_this());
    }
}
void TcpConnection::handleCloseCallbacks(){
    if(_onConnction){
        _onClose(shared_from_this());
    }
}

}//end of namespace lg
