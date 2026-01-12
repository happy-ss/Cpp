#ifndef __SOCKET_HPP__
#define __SOCKET_HPP__

namespace lg{

class Socket{
private:
    int _fd;
public:
    Socket();
    Socket(int fd);
    ~Socket();
    int fd();
    void shutdownWrite();

};//end of namespace lg
}


#endif

