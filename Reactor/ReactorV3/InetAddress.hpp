#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__
#include <header.h>
#include <string>
using std::string;
namespace lg{

class InetAddress{
private:
    struct sockaddr_in _addr;
public:
    InetAddress(unsigned short port,const char *ip="0.0.0.0");
    InetAddress(const struct sockaddr_in& addr);
    ~InetAddress(){}
    string ip();
    unsigned short port();
    struct sockaddr_in * getSockaddrinPtr();
};


}//end of namespace lg
#endif

