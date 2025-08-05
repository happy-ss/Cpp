#include "ThreadPool.hpp"
#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;
namespace lg{

ThreadPool::ThreadPool(int threadNum,int capa)
:_threadNum(threadNum),
_threads(),
_capacity(capa),
_taskQue(_capacity),
_isExit(false)
{
    _threads.reserve(_threadNum);
}
void ThreadPool::start(){
    for(int i=0;i<_threadNum;++i){
        unique_ptr<Thread>  th(new Thread(std::bind(&ThreadPool::doTask,this)));
        _threads.push_back(std::move(th));
    }
    for(auto&thread:_threads){
        thread->start();
    }
}
void ThreadPool::addTask(Task &&task){
    if(task){
        _taskQue.push(task);
    }
}
void ThreadPool::doTask(){
    while(!_isExit){
        Task task=_taskQue.pop();
        if(task){
            task();
        }
    }
}

void ThreadPool::stop(){
    while(!_taskQue.empty()){
        sleep(1);
    }
    _isExit=true;
    _taskQue.wakeUp();
    for(auto &thread:_threads){
        thread->join();
    }
}


}



