#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__
#include "Socket.hpp"
#include "SocketIO.hpp"
#include "InetAddress.hpp"
#include <functional>
#include <memory>
namespace lg{
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using Callback=std::function<void(TcpConnectionPtr)>;
class TcpConnection:public std::enable_shared_from_this<TcpConnection>{
public:
    TcpConnection(int fd);
    void send(const string&);
    string recv();
    void shutdown();
    string toString();
    bool isClosed();
    void setAllCallbacks(const Callback &cb1,
                         const Callback &cb2,
                         const Callback &cb3);
    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();
private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
private:
    Socket _sock;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    SocketIO _socketIO;
    Callback _onConnection;
    Callback _onMessage;
    Callback _onClose;
};







}





#endif

