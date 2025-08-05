#include "EventLoop.hpp"
#include <iostream>
using std::cout;
using std::endl;
using lg::TcpConnectionPtr;
void onConnection(TcpConnectionPtr connptr){
    cout<<connptr->toString()<<"has connected."<<endl;
}
void onMessage(TcpConnectionPtr connptr){
    string msg=connptr->recv();
    cout<<"recv:"<<msg<<endl;
    //decode
    //compute
    //encode
    
    connptr->send(msg);
}
void onClose(TcpConnectionPtr connptr){
    cout<<connptr->toString()<<"has closed."<<endl;
}

int main(){
    lg::Acceptor acceptor(8080);
    acceptor.ready();
    lg::EventLoop loop(acceptor);
    loop.setAllCallBacks(onConnection,
                         onMessage,
                         onClose);
    loop.loop();

    return 0;
}
