#include "Thread.hpp"

namespace  lg{

Thread::Thread(ThreadCallBack &&cb)
    :_pthid(0),
    _isRunning(false),
    _threadCallBack(std::move(cb))
{

}
void Thread::start(){
    if(!_isRunning){
        _isRunning=true;
        pthread_create(&_pthid,NULL,start_routine,this);
    }
}
void* Thread::start_routine(void *arg){
    Thread* pth=(Thread*)arg;
    pth->_threadCallBack();
    return NULL;
}
void Thread::join(){
    if(_isRunning){
        _isRunning=false;
        pthread_join(_pthid,NULL);
    }
}
}
