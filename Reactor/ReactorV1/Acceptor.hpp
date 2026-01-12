#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__
#include <header.h>
#include "Socket.hpp"
#include "InetAddress.hpp"
namespace lg{

class Acceptor{
public:
    Acceptor(unsigned short port,const char* ip="0.0.0.0");
    ~Acceptor(){};
    void ready();
    int accept();
private:
    void bind();
    void listen();
    void setReuseAddr(bool);
    void setReusePort(bool);
private:
    Socket _sock;
    InetAddress _inetAddr;
};


}



#endif

