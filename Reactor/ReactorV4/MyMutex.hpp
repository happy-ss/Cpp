#ifndef __MYMUTEX_H__
#define __MYMUTEX_H__
#include <pthread.h>
class MyMutex{
public:
    MyMutex();
    ~MyMutex();
    void lock();
    void unlock();
    pthread_mutex_t* getMutexPtr();
private:
    pthread_mutex_t _mutex;
};

class MutexGuard{
public:
    MutexGuard(MyMutex&);
    ~MutexGuard();
private:
    MyMutex& _mutex;
};
#endif

