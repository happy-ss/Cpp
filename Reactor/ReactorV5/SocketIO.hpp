#ifndef __SOCKETIO_H__
#define __SOCKETIO_H__
#include <header.h>
namespace lg{

class SocketIO{
private:
    int _fd;
public:
    SocketIO(int fd);
    ~SocketIO(){}
    int recvn(char *buf,int len);
    int sendn(const char *buf,int len);
    int recvLine(char *buf,int maxlen);
    int recvPeek(char *buf,int len);
};




}//end of namespace lg

#endif

