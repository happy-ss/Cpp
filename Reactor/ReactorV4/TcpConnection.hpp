#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__
#include "Socket.hpp"
#include "SocketIO.hpp"
#include "InetAddress.hpp"
#include <functional>
#include <memory>
namespace lg{
class EventLoop;
class TcpConnection;
using TcpConnectionPtr=std::shared_ptr<TcpConnection>;
using Callback=std::function<void(TcpConnectionPtr)>;

class TcpConnection:public std::enable_shared_from_this<TcpConnection>{
private:
    Socket      _sock;
    SocketIO    _sockIO;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    Callback    _onConnction;
    Callback    _onMessage;
    Callback    _onClose;
    EventLoop*  _eventLoop;
public:
    TcpConnection(int fd,EventLoop*);
    ~TcpConnection(){}
    string toString();
    string recv();
    void send(const string &msg);
    void sendInLoop(const string &msg);
    bool isClosed();
    void shutdown();
    void setAllCallbacks(const Callback &cb1,
                         const Callback &cb2,
                         const Callback &cb3){
        _onConnction=cb1;
        _onMessage=cb2;
        _onClose=cb3;
    }
    void handleConnectionCallbacks();
    void handleMessageCallbacks();
    void handleCloseCallbacks();
private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
};

}//end of namespace lg



#endif

