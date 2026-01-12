#ifndef __SOCKET_H__
#define __SOCKET_H__
#include <header.h>

namespace lg{
class Socket{
public:
    Socket();
    Socket(int);
    ~Socket();
    int fd();
    void shutdownWrite();
private:
    int _fd;
};
}


#endif

