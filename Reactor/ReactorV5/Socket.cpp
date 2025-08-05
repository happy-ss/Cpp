#include "Socket.hpp"
#include <header.h>


namespace lg{

Socket::Socket(){
    _fd=socket(AF_INET,SOCK_STREAM,0);
}

Socket::Socket(int fd)
:_fd(fd){

}
Socket::~Socket(){
    close(_fd);
}
int Socket::fd(){
    return _fd;
}
void Socket::shutdownWrite(){
    shutdown(_fd,SHUT_WR);
}
}//end of namespace lg
