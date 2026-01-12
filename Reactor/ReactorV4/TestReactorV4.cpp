#include "TcpServer.hpp"
#include "ThreadPool.hpp"
#include <iostream>
using std::cout;
using std::endl;
lg::ThreadPool threadPool(4,10);
class MyTask{
public:
    MyTask(const string& msg,lg::TcpConnectionPtr conn)
    :_msg(msg)
    ,_conn(conn){}
    void process(){
        cout<<"MyTask::process is running"<<endl;
        string response=_msg;
        _conn->sendInLoop(_msg);
    }
private:
    string _msg;
    lg::TcpConnectionPtr _conn;
};
void onConnection(lg::TcpConnectionPtr tcp){
    cout<<"tcp  "<<tcp->toString()<<"  has connected."<<endl;
}
void onMessage(lg::TcpConnectionPtr tcp){
    string msg=tcp->recv();
    MyTask task(msg,tcp);
    threadPool.addTask(std::bind(&MyTask::process,task));
    string response=msg;
    tcp->send(response);
}
void onClose(lg::TcpConnectionPtr tcp){
    cout<<"tcp  "<<tcp->toString()<<"  has closed."<<endl;
}
int main(){
    threadPool.start();
    lg::TcpServer tcp(8080);
    tcp.setAllCallbacks(onConnection,onMessage,onClose);
    tcp.start();
    /* tcp.stop(); */
    /* threadPool.stop(); */
    return 0;
}
