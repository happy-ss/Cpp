#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__
#include "Acceptor.hpp"
#include "TcpConnection.hpp"
#include <iostream>
#include <vector>
#include <map>
using std::vector;
using std::map;
namespace  lg{
class EventLoop{
public:
    EventLoop(Acceptor& acceptor);
    ~EventLoop();
    void setAllCallBacks(Callback &&cb1,
                        Callback &&cb2,
                        Callback &&cb3){
        _onConnection=std::move(cb1);
        _onMessage=std::move(cb2);
        _onClose=std::move(cb3);
    }
    void loop();
    void unloop();
private:
    int creatEpollFd();
    void addEpollReadEvent(int);
    void delEpollReadEvent(int);
    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int);
private:
    int _epfd;
    Acceptor & _acceptor;
    bool _isLoop;
    vector<struct epoll_event> _eveArr;
    map<int,TcpConnectionPtr> _conns;
    Callback _onConnection;
    Callback _onMessage;
    Callback _onClose;
};


}//end of namespace lg
#endif

