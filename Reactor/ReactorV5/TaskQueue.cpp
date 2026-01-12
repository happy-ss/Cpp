#include "TaskQueue.hpp"
#include "MyCondition.hpp"
#include "MyMutex.hpp"

TaskQueue::TaskQueue(int capa )
:_que(),
_capacity(capa),
_mutex(),
_notEmpty(_mutex),
_notFull(_mutex),
_flag(true){

}

TaskQueue::~TaskQueue(){

}
bool TaskQueue::empty(){
    return _que.size()==0;
}
bool TaskQueue::full(){
    return _que.size()==(size_t)_capacity;
}

void TaskQueue::push(Task task){
    MutexGuard mg(_mutex);
    while(full()){
        _notFull.wait();
    }
    _que.push(task);
    _notEmpty.notifyOne();
}
TaskQueue::Task TaskQueue::pop(){
    MutexGuard mg(_mutex);
    while(_flag&&empty()){
        _notEmpty.wait();
    }
    Task temp=_que.front();
    if(temp){
        _que.pop();
        _notFull.notifyOne();
    }
    return temp;
}
void TaskQueue::wakeUp(){
    _flag=false;
    _notEmpty.notifyAll();
}

