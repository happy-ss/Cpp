#ifndef __MYCONDITION_H__
#define __MYCONDITION_H__
#include <pthread.h>

class MyMutex;
class MyCondition{
public:
    MyCondition(MyMutex &m);
    ~MyCondition();
    void wait();
    void notifyOne();
    void notifyAll();
private:
    pthread_cond_t _cond;
    MyMutex & _mutex;
};
#endif

