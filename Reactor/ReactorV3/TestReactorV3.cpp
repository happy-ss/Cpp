#include "TcpServer.hpp"
#include <iostream>
using std::cout;
using std::endl;
using TcpConnectionPtr=std::shared_ptr<lg::TcpConnection>;
void onConnection(TcpConnectionPtr tcp){
    cout<<"tcp  "<<tcp->toString()<<"  has connected."<<endl;
}
void onMessage(TcpConnectionPtr tcp){
    string msg=tcp->recv();
    tcp->send(msg);
}
void onClose(TcpConnectionPtr tcp){
    cout<<"tcp  "<<tcp->toString()<<"  has closed."<<endl;
}
int main(){
    lg::TcpServer tcp(8080);
    tcp.setAllCallbacks(onConnection,onMessage,onClose);
    tcp.start();
    tcp.stop();
    return 0;
}
