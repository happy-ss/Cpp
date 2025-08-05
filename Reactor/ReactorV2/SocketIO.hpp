#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__
#include <header.h>

namespace  lg{

class SocketIO{
public:
    SocketIO(int fd)
    :_fd(fd)
    {}

    int readn(char* buf,int len);
    int sendn(const char* buf,int len);
    int readLine(char *buf,int maxLen);
    int recvPeek(char* buf,int len);
private:
    int _fd;

};







}


#endif

