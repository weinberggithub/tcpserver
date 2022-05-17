#include <sys/shm.h>
#include <unistd.h>
#include "shmtx.h"
#include<iostream>
using namespace std;
shmtx::shmtx(){
    int id = shmget(IPC_PRIVATE, 128, (SHM_R|SHM_W|IPC_CREAT));
    if(id == -1){
        throw "shmget failed.";
    }
    _locker = (std::atomic_int*)shmat(id, NULL, 0);
    if(_locker == (void*) -1){
        throw "shmat failed.";
    }
    *_locker = 0;
    _wait = _locker + 1;
    *_wait = 0;
}

shmtx::~shmtx(){
   _locker = nullptr;
    _wait = nullptr;
}

void 
shmtx::Lock(){
    while(1){
        int expected = 0;
        if(*_locker == 0 && std::atomic_compare_exchange_strong(_locker,&expected,1)){
            //cout<<getpid()<<"  get locker"<<endl;
            return;
        }

        std::atomic_fetch_add(_wait,1);
        //cout<<getpid()<<"  waittin in locker"<<endl;
        _sem.Wait();
        //cout<<getpid()<<"  awoke"<<endl;
        usleep(1);
    }
    return;
}

bool 
shmtx::TryLock(){
   int expected = 0;
   return (*_locker == 0 && std::atomic_compare_exchange_strong(_locker,&expected,1));
}

bool 
shmtx::LockWithTimeout(uint32 timeout){
    do{
        int expected = 0;
        if(*_locker == 0 && std::atomic_compare_exchange_strong(_locker,&expected,1)){
            // cout<<"get locker"<<endl;
            return true;
        }
        // cout<<"get locker failed and wait"<<endl;
        _sem.WaitTimeout(timeout);
        // cout<<"get locker failed and awoken"<<endl;
    }while(0);
    // cout<<"get locker failed and return false"<<endl;
    return false;
}

void 
shmtx::Unlock(){
    int expected = 1;
     if (std::atomic_compare_exchange_strong(_locker,&expected,0)) {
        for (;;){
            //cout<<getpid()<<"  wait minus 1 begin"<<endl;
            auto wait = (int)*_wait;
            if(wait <= 0) return;
            if(std::atomic_compare_exchange_strong(_wait,&wait,wait - 1)){
                //cout<<getpid()<<"  wait minus 1 auccess"<<endl;
                break;
            }
        }
        _sem.Signal();
        //cout<<getpid()<<"  signal success"<<endl;
    }
    //cout<<getpid()<<"  unlock success"<<endl;
}