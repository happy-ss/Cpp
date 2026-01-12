#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__
#include "Socket.hpp"
#include "SocketIO.hpp"
#include "InetAddress.hpp"

namespace lg{

class TcpConnection{
public:
    TcpConnection(int fd);
    void send(const string&);
    string recv();
    void shutdown();
    string toString();
    bool isClosed();
private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
private:
    Socket _sock;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    SocketIO _socketIO;
};







}





#endif

