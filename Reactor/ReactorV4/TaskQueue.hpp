#ifndef __TASKQUEUE_H__ 
#define __TASKQUEUE_H__
#include "MyMutex.hpp"
#include "MyCondition.hpp"
#include <queue>
#include <functional>
using std::queue;
using std::function;
class TaskQueue{
    using Task=function<void()>;
public:
    TaskQueue(int capa);
    ~TaskQueue();
    bool empty();
    bool full();
    void push(Task);
    Task pop();
    void wakeUp();
private:
    queue<Task> _que;
    int _capacity;
    MyMutex _mutex;
    MyCondition _notEmpty;
    MyCondition _notFull;
    bool _flag;
};



#endif

