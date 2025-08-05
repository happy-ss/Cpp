#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__
#include <header.h>
#include <string>
using std::string;

namespace lg{
class InetAddress{
public:
    InetAddress(unsigned short port,const char* ip="0.0.0.0");
    InetAddress(struct sockaddr_in &);
    ~InetAddress(){}
    string ip();
    unsigned short port();
    struct sockaddr_in* getSockaddrPtr();
private:
    struct sockaddr_in _addr;
};


}//end of namespace lg
#endif

