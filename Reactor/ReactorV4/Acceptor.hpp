#ifndef __ACCEPTOR_HPP__
#define __ACCEPTOR_HPP__
#include "Socket.hpp"
#include "InetAddress.hpp"
namespace lg{
class Acceptor{
private:
    InetAddress _addr;
    Socket      _sock;
    

public:
    Acceptor(unsigned short port,const char* ip="0.0.0.0");
    ~Acceptor();
    int accept();
    void ready();
    int fd();
private:
    void setReuseAddr();
    void setReusePort();
    void bind();
    void listen();
};


}//end of namespace lg




#endif

