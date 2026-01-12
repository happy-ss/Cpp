#include "MyMutex.hpp"

MyMutex::MyMutex()
{
    pthread_mutex_init(&_mutex,nullptr);
}
MyMutex::~MyMutex(){
    pthread_mutex_destroy(&_mutex);
}
void MyMutex::lock(){
    pthread_mutex_lock(&_mutex);
}
void MyMutex::unlock(){
    pthread_mutex_unlock(&_mutex);
}

pthread_mutex_t* MyMutex::getMutexPtr(){
    return &_mutex;
}
MutexGuard::MutexGuard(MyMutex & m)
:_mutex(m){
    _mutex.lock();
}
MutexGuard::~MutexGuard(){
    _mutex.unlock();
}



