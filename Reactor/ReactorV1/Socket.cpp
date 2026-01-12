#include "Socket.hpp"

namespace lg{

Socket::Socket(){
    _fd=socket(AF_INET,SOCK_STREAM,0);
    if(_fd<0){
        perror("socket");
    }
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

}








