#include "MyCondition.hpp"
#include "MyMutex.hpp"
MyCondition::MyCondition(MyMutex& m)
:_mutex(m){
    pthread_cond_init(&_cond,nullptr);
}
MyCondition::~MyCondition(){
    pthread_cond_destroy(&_cond);
}

void MyCondition::wait(){
    pthread_cond_wait(&_cond,_mutex.getMutexPtr());
}

void MyCondition::notifyOne(){
    pthread_cond_signal(&_cond);
}
void MyCondition::notifyAll(){
    pthread_cond_broadcast(&_cond);
}


