#include "TcpServer.hpp"
#include "ThreadPool.hpp"
#include <iostream>
using std::cout;
using std::endl;
class MyTask{
public:
    MyTask(const string& msg,lg::TcpConnectionPtr conn)
    :_msg(msg)
    ,_conn(conn)
    {}
    void process(){
        cout<<"MyTask::process is running"<<endl;
        string response=_msg;
        _conn->sendInLoop(_msg);
    }
private:
    string _msg;
    lg::TcpConnectionPtr _conn;
};
class EchoServer{
private:
    lg::ThreadPool _threadPool;
    lg::TcpServer _tcpServer;
public:
    EchoServer(int threadNum,int queCapacity,unsigned short port,const char* ip="0.0.0.0")
        :_threadPool(threadNum,queCapacity)
        ,_tcpServer(port,ip)
        {
            using namespace std::placeholders;
            _tcpServer.setAllCallbacks(std::bind(&EchoServer::onConnection,this,_1),
                                       std::bind(&EchoServer::onMessage,this,_1),
                                       std::bind(&EchoServer::onClose,this,_1));
        }
    void start(){
        _threadPool.start();
        _tcpServer.start();
    }
    void stop(){
        _threadPool.stop();
        _tcpServer.stop();
    }
private:
    void onConnection(lg::TcpConnectionPtr tcp){
        cout<<"tcp  "<<tcp->toString()<<"  has connected."<<endl;
    }
    void onMessage(lg::TcpConnectionPtr tcp){
        string msg=tcp->recv();
        MyTask task(msg,tcp);
        _threadPool.addTask(std::bind(&MyTask::process,task));
        //decode
        //compyte
        //encode
        //在此添加业务逻辑
        string response=msg;
        tcp->send(response);
    }
    void onClose(lg::TcpConnectionPtr tcp){
        cout<<"tcp  "<<tcp->toString()<<"  has closed."<<endl;
    }


};
int main(){
    EchoServer echoServer(4,10,8080);
    echoServer.start();
    echoServer.stop();
    return 0;
}
