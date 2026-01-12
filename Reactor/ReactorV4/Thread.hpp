#ifndef __THREAD_H__
#define __THREAD_H__
#include <pthread.h>
#include <functional>
using std::function;
namespace lg{

class Thread{
    using ThreadCallBack= function<void()>;
public:
    Thread(ThreadCallBack&& cb);
    ~Thread(){}
    void start();
    void join();
    static void* start_routine(void*);
private:
    pthread_t _pthid;
    bool _isRunning;
    ThreadCallBack _threadCallBack;
};

}
#endif

