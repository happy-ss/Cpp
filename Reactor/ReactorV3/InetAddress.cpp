#include "InetAddress.hpp"

namespace lg{

InetAddress::InetAddress(unsigned short port,const char* ip){
    _addr.sin_family=AF_INET;
    _addr.sin_port=htons(port);
    _addr.sin_addr.s_addr=inet_addr(ip);
}
InetAddress::InetAddress(const struct sockaddr_in & addr)
:_addr(addr){

}
string InetAddress::ip(){
    return inet_ntoa(_addr.sin_addr);
}

unsigned short InetAddress::port(){
    return ntohs(_addr.sin_port);
}
struct sockaddr_in * InetAddress::getSockaddrinPtr(){
    return &_addr;
}
}//end of namespace lg
