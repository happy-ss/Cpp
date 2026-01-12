#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__
#include "Acceptor.hpp"
#include "TcpConnection.hpp"
#include <vector>
#include <map>
using std::vector;
using std::map;
namespace lg{

class EventLoop{

using TcpConnectionPtr=std::shared_ptr<TcpConnection>;
using Callback=std::function<void(TcpConnectionPtr)>;
private:
    int _epfd;
    Acceptor & _acceptor;
    bool _isLooping;
    vector<struct epoll_event> _events;
    map<int,TcpConnectionPtr> _conns;
    Callback _onConnction;
    Callback _onMessage;
    Callback _onClose;
public:
    EventLoop(Acceptor &acceptor);
    ~EventLoop(){}
    int createEpollFd();
    void loop();
    void unloop();
    void setAllCallbacks(Callback &&cb1,
                         Callback &&cb2,
                         Callback &&cb3){
        _onConnction=std::move(cb1);
        _onMessage=std::move(cb2);
        _onClose=std::move(cb3);
    }
private:
    void addEpollReadEvent(int);
    void delEpollReadEvent(int);
    void waitEpollFd();
    void handleConnetcion();
    void handleMessage(int);
    void handleClose();
};


}//end of namespace lg

#endif

