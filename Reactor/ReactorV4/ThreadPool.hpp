#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include "TaskQueue.hpp"
#include "Thread.hpp"
#include <memory>
#include <vector>
using std::unique_ptr;
using std::vector;
namespace lg{
class Thread;
class ThreadPool{
    friend class WorkThread;
    using Task=function<void()>;
public:
    ThreadPool(int,int);
    ~ThreadPool(){}
    void start();
    void stop();
    void addTask(Task&&task);
private:
    void doTask();
private:
    int _threadNum;
    vector<unique_ptr<Thread>> _threads;
    int _capacity;
    TaskQueue _taskQue;
    bool _isExit;
};
}
#endif

