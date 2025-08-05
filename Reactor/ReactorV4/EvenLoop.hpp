#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__
#include "Acceptor.hpp"
#include "TcpConnection.hpp"
#include "MyMutex.hpp"
#include <vector>
#include <map>
using std::vector;
using std::map;
namespace lg{
using Functors=std::function<void()>;
using TcpConnectionPtr=std::shared_ptr<TcpConnection>;
using Callback=std::function<void(TcpConnectionPtr)>;
class EventLoop{

private:
    int _epfd;
    int _eventfd;
    Acceptor & _acceptor;
    bool _isLooping;
    vector<struct epoll_event> _events;
    vector<Functors> _pendingFunctors;
    map<int,TcpConnectionPtr> _conns;
    Callback _onConnction;
    Callback _onMessage;
    Callback _onClose;
    MyMutex _mutex;
public:
    EventLoop(Acceptor &acceptor);
    ~EventLoop(){}
    void loop();
    void unloop();
    void setAllCallbacks(Callback &&cb1,
                         Callback &&cb2,
                         Callback &&cb3){
        _onConnction=std::move(cb1);
        _onMessage=std::move(cb2);
        _onClose=std::move(cb3);
    }
    void runInLoop(Functors &&);
private:
    void addEpollReadEvent(int);
    void delEpollReadEvent(int);
    void waitEpollFd();
    void handleConnetcion();
    void handleMessage(int);
    void handleClose();
    int createEpollFd();
    int createEventFd();
    void handleReadEvent();
    void wakeup();
    void doPendingFunctors();
};


}//end of namespace lg

#endif

