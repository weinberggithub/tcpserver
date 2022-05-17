#include<errno.h>
#include "sem.h"

semaphore::semaphore(bool shared){
    int ret = sem_init(&_sem,(int)shared,SEM_INIT_VAL);
    if(ret == -1){
        //todo failed fetch errno to log
        throw "sem_init failed";
    }

}

semaphore::~semaphore(){
    sem_destroy(&_sem);
}

void 
semaphore::Wait(){
    int ret = sem_wait(&_sem);
    if(ret == -1){
        throw "sem_wait failed.";
    }
}

void 
semaphore::Signal(){
    int ret =  sem_post(&_sem);
    if(ret == -1){
        throw "sem_post failed.";
    }
}

void 
semaphore::WaitTimeout(uint32 timeout){
    struct timespec ts;
    ts.tv_nsec = (long)(timeout * 1000000);
    int ret = sem_timedwait(&_sem,&ts);
    if(ret == -1 && errno != ETIMEDOUT){
        throw "sem_wait failed.";
    }
}