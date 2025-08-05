#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__
#include "TcpConnection.hpp"
#include "Acceptor.hpp"
#include "EvenLoop.hpp"
namespace lg{

class TcpServer{
    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using Callback=std::function<void(TcpConnectionPtr)>;
private:
    Acceptor _acceptor;
    EventLoop _loop;
public:
    TcpServer(unsigned short port,const char*ip="0.0.0.0")
    :_acceptor(port,ip),
    _loop(_acceptor){

    }
    ~TcpServer(){}
    void start(){
        _acceptor.ready();
        _loop.loop();
    }
    void stop(){
        _loop.unloop();
    }
    void setAllCallbacks(Callback &&cb1,
                         Callback &&cb2,
                         Callback &&cb3){
        _loop.setAllCallbacks(std::move(cb1),
                              std::move(cb2),
                              std::move(cb3));
    }
};




}//end of namespace lg

#endif

