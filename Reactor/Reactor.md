# Reactor

## ReactorV1

可以与一个客户端进行简单交互，如回显。

InetAddress类：封装ip地址和端口，主机网络大小端转化

| InetAddress                                             |
| :------------------------------------------------------ |
| -_sock:struct sockaddr_in ;                             |
|                                                         |
| +InetAddress(port : unsigned short , ip : const char*); |
| +InetAddress(const struct sockaddr_in&);                |
| +~InetAddress();                                        |
| ip(): string                                            |
| port():unsigned short                                   |
| getSockaddrPtr():struct sockadddr_in*                   |

Socket类：RAII思想封装socket套接字，构造函数申请资源，析构函数释放资源

Socket不能进行复制控制操作，可以继承Nocopyable类

| Socket                                          |
| :---------------------------------------------- |
| -_sockfd : int                                  |
|                                                 |
| +Socket();                                      |
| +Socket(int fd);                                |
| +~Socket();                                     |
| fd(): int                                       |
| shutWrite():void   //调用shutdown()函数关闭写端 |

Acceptor类：封装bind、listen、accept。 

Acceptor类包含Socket InetAddress 子对象。因为bind需要sockfd,以及ip port。

| Acceptor                                                     |
| :----------------------------------------------------------- |
| -_sock : Socket                                              |
| -_addr : InetAddress                                         |
|                                                              |
| +Acceptor(port : unsigned short , ip : const char*);         |
| +~Acceptor();                                                |
| +fd():int                                                    |
| -setReuseAddr():void                                         |
| -setReusePort():void                                         |
| -bind():void   //调用bind()绑定，<span style=color:red;background:yellow;font-size:16px>第二个参数需要sockaddr_in的地址，所以在InetAddress中加getSockaddrPtr()函数</span> |
| -listen():void                                               |
| +ready():void  // 调用以上四个函数                           |
| +accept():int                                                |

SocketIO类：封装发送接收数据等IO操作 

| SocketIO                                                     |
| :----------------------------------------------------------- |
| -_fd : int                                                   |
|                                                              |
| +SocketIO();                                                 |
| +~SocketIO();                                                |
| +recvn(buf : char*,len : int):int                            |
| +sendn(buf : cosnt char*,len : int):int                      |
| +recvPeek(buf : char*,len : int):int     //使用MSG_PEEK标志位，复制但不清除内核缓冲区数据 |
| +recvLine(buf : char*,maxlen : int):int                      |

TcpConnection类：表示一个建立好的TCP连接。

TcpConnetion包含Socket、InetAddress、SocketIO子对象。

包含Socket：有新连接时，需要在内核中构造socket文件对象，<span style=color:red;background:yellow;font-size:16px>所以Socket还需要一个Socket(int fd)构造函数</span>

包含InetAddress：需要有本地地址、对端地址。

包含SocketIO：需要进行数据发送接收。

| TcpConnetion                                                 |
| :----------------------------------------------------------- |
| -_sock : Socket                                              |
| -_sockIO : SocketIO                                          |
| -_localAddr : InetAddress                                    |
| -_peerAddr : InetAddress                                     |
| -_onConnecion:Callback          //using Callback=function<void(TcpConnectionPtr)> |
| -_onMessage:Callback                                         |
| -_onMessage:Callback                                         |
|                                                              |
| +TcpConnection(int fd);   //调用Acceptor::accept来构造       |
| +~TcpConnection();                                           |
| -getLocalAddress():InetAddress   //调用setsockname() 函数获得本机地址，来初始化_localAddr<br />最后返回时需要根据sockaddr_in 来构造InetAddress对象，<span style=color:red;background:yellow;font-size:16px>所以需要InetAddress(const struct sockaddr_in&)构造函数</span>否则ip默认为"0.0.0.0"，端口会再进行大小端转化一次。 |
| -getPeerAddress():InetAddress   //调用setpeername() 函数获得对端地址，来初始化_peerAddr |
| +recv():string                                               |
| +send(cosnt string &):void                                   |
| +toString():string   //打印网络连接信息                      |
| +shutdown():void                                             |

## ReactorV2

加入epoll,可以实现IO多路复用

TcpConnection一般都会有三件事要处理：1.连接加入 2.收到信息 3.连接退出

还不确定要怎么处理，在测试文件里写出具体操作。因此可以使用回调函数

| TcpConnetion                                                 |
| :----------------------------------------------------------- |
| -_sock : Socket                                              |
| -_sockIO : SocketIO                                          |
| -_localAddr : InetAddress                                    |
| -_peerAddr : InetAddress                                     |
| -_onConnecion:Callback      //using TcpConnectionPtr = shared_ptr<TcpConnection>; |
| -_onMessage:Callback        //using Callback=function<void(TcpConnectionPtr)>; |
| -_onClose:Callback             //<span style=color:red;background:yellow;font-size:16px>执行回调时需要知道是哪个TcpConnection，所以要有参数TcpConnectionPtr</span> |
|                                                              |
| +TcpConnection(int fd);                                      |
| +~TcpConnection();                                           |
| -getLocalAddress():InetAddress                               |
| -getPeerAddress():InetAddress                                |
| +recv():string                                               |
| +send(cosnt string &):void                                   |
| +toString():string                                           |
| +shutdown():void                                             |
| +setAllCallbacks(cb1:const Callbacks&,cb2,cb3):void  //注册回调，不能使用移动构造，因为TcpConnection 有多个 |
| +ConnectionCallback():void    //执行回调   因为Callback的形式是void(TcpConnectionPtr) |
| +MessageCallback():void       //<span style=color:red;background:yellow;font-size:16px>需要一个指向TcpConnection的shared_ptr，不可以_onConnection(this)</span> |
| +CloseCallback():void            //<span style=color:red;background:yellow;font-size:16px>两个智能指针托管同一个对象，会造成double free</span><br />                                      //<span style=color:red;background:yellow;font-size:16px>让TcpConnection类public继承std::enable_shared_from_this<>，用shared_from_this代替this</span> |

EventLoop类：封装epoll的各种操作，并进行事件循环。

| EventLoop                                                    |
| :----------------------------------------------------------- |
| -_epfd:int                                                   |
| -_acceptor:Acceptor&                                         |
| -_conns:map<int,TcpConnectionPtr>                            |
| -_isLooping:bool                                             |
| -_events:vector<struct epoll_event>                          |
| -_onConnecion:Callback      //using TcpConnectionPtr = shared_ptr<TcpConnection>; |
| -_onMessage:Callback        //using Callback=function<void(TcpConnectionPtr)>; |
| -_onClose:Callback                                           |
| //TcpConnection需要注册回调，但无法进行传参，所以可以使用数据成员作为中转传递 |
|                                                              |
| +EventLoop(Acceptor &acceptor);                              |
| +~EventLoop();          //close(_epfd)                       |
| -createEpollFd():int                                         |
| -addReadEvent():void                                         |
| -delReadEvent():void                                         |
| +loop():void                                                 |
| -waitEpollFd():void                                          |
| -handleConnection():void                                     |
| -handleMessage():void                                        |
| -handleClose():void                                          |
| +setAllCallbacks(cb1: Callbacks&&,cb2,cb3):void              |

## ReactorV3

封装EventLoop类和Acceptor类，对外表现为TcpServer。

| TcpServer                                                |
| :------------------------------------------------------- |
| -_acceptor:Acceptor                                      |
| -_eventLoop:EventLoop                                    |
|                                                          |
| +TcpServer(unsigned short port,const char* ip);          |
| +~TcpServer();                                           |
| +start():void     //_acceptor.ready()  _eventLoop.loop() |
| +setAllCallbacks(Callback&&,Callback&&,Callback&&):void  |
| +stop():void                                             |



## ReactorV4

V3版本执行任务是串行的，效率太低。加入线程池，并发执行。

线程池中的线程是计算线程，进行计算工作，但不进行IO操作。发送接收数据等操作由IO线程执行。

```c++
class MyTask{
public:
    MyTask(const string &msg,TcpConnectionPtr conn)
    :_msg(msg)
    ,_conn(conn)
    {}
    void process(){
  		string response = msg; //执行回显操作
        _conn->sendInLoop(response);//通知IO进程
    }
private:
    string _msg;
    TcpConnectionPtr _conn;
};
```



| TcpConnetion                                                 |
| :----------------------------------------------------------- |
| -_sock : Socket                                              |
| -_sockIO : SocketIO                                          |
| -_localAddr : InetAddress                                    |
| -_peerAddr : InetAddress                                     |
| -_onConnecion:Callback      //using TcpConnectionPtr = shared_ptr<TcpConnection>; |
| -_onMessage:Callback        //using Callback=function<void(TcpConnectionPtr)>; |
| -_onClose:Callback                                           |
| -_eventLoop:EventLoop*                                       |
|                                                              |
| +TcpConnection(int fd);                                      |
| +~TcpConnection();                                           |
| -getLocalAddress():InetAddress                               |
| -getPeerAddress():InetAddress                                |
| +recv():string                                               |
| +send(cosnt string &):void                                   |
| +toString():string                                           |
| +shutdown():void                                             |
| +setAllCallbacks(cb1:const Callbacks&,cb2,cb3):void          |
| +ConnectionCallback():void                                   |
| +MessageCallback():void                                      |
| +CloseCallback():void                                        |
| +sendInLoop(const string&msg):void   //计算线程处理完msg 通知IO线程发送，调用runInLoop(Functor&&)<br />                                                              //传参bind(&TcpConnection::send,this,msg) 注册回调，把send函数打包 |



| EventLoop                                                    |
| :----------------------------------------------------------- |
| -_epfd:int                                                   |
| -_eventfd:int                                                |
| -_acceptor:Acceptor&                                         |
| -_conns:map<int,TcpConnectionPtr>                            |
| -_isLooping:bool                                             |
| -_events:vector<struct epoll_event>                          |
| -_functors:vector<Functor>                                   |
| -_onConnecion:Callback      //using TcpConnectionPtr = shared_ptr<TcpConnection>; |
| -_onMessage:Callback        //using Callback=function<void(TcpConnectionPtr)>; |
| -_onClose:Callback                                           |
| -_mutex:MyMutex              //多线程条件下往vector里添加元素和遍历元素时，加锁 |
|                                                              |
| +EventLoop(Acceptor &acceptor);    //将_epfd,_eventfd加入监听 |
| +~EventLoop();                    //close(_epfd)   close(_eventfd) |
| -createEpollFd():int                                         |
| -createEventFd():int                                         |
| -addReadEvent():void                                         |
| -delReadEvent():void                                         |
| +loop():void                                                 |
| -waitEpollFd():void                       //新增_eventfd就绪的判断，就绪调用handleEventfd(),doPendingFunctors(); |
| -handleConnection():void                                     |
| -handleMessage():void                                        |
| -handleClose():void                                          |
| +setAllCallbacks(cb1: Callbacks&&,cb2,cb3):void              |
| +runInLoop(Functor &&f):void     //using Functor=function<void()>;  //将f存入vector,并调用wakeup通知IO线程 |
| -handleEventfd():void                                        |
| -doPendingFunctors():void     //遍历vector,执行回调函数  //vector<Functor> tmp; <span style=color:red;background:yellow;font-size:16px>tmp.swap(_pendingFunctors);O(1)</span> |
| -wakeup():void                                               |



## ReactorV5

将TcpServer和ThreadPool封装到一起，对外显示为EchoServer。

| EchoServer                                                   |
| :----------------------------------------------------------- |
| -_threadPool:ThreadPool                                      |
| -_tcpServer:TcpServer                                        |
|                                                              |
| +EchoServer(int threadNum,int queCapacity,unsigned short port,const char* ip); |
| +~EchoServer();                                              |
| +start():void          //_tcpServer.setAllCallBacks(std::bind(&EchoServer::onConnection,this,__1)); |
| +stop():void                                                 |
| -onConnection():void                                         |
| -onMessage():void                                            |
| -onClose():void                                              |