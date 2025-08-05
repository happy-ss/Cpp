#include "TcpConnection.hpp"
#include "Acceptor.hpp"
#include <iostream>
using std::cout;
using std::endl;

using namespace lg;

int main(){
    Acceptor acceptor(8080);
    acceptor.ready();
    TcpConnection tcp(acceptor.accept());
    cout<<tcp.toString()<<"has connected."<<endl;
    
    string msg=tcp.recv();
    cout<<msg<<endl;
    
    tcp.send(msg);
    tcp.shutdown();
    return 0;
}
