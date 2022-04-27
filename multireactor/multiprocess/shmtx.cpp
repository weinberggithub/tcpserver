#include <sys/shm.h>

#include "shmtx.h"

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
            return;
        }

        std::atomic_fetch_add(_wait,1);
        _sem.Wait();
        //cout<<"awoke"<<endl;
    }
    //cout<<"bad thing happends"<<endl;
    return;     
}

bool 
shmtx::TryLock(){
   int expected = 0;
   return (*_locker == 0 && std::atomic_compare_exchange_strong(_locker,&expected,1));
}

void 
shmtx::Unlock(){
    int expected = 0;
     if (std::atomic_compare_exchange_strong(_locker,&expected,0)) {
        for (;;){
            auto wait = (int)*_wait;
            if(wait <= 0) return;
            if(std::atomic_compare_exchange_strong(_wait,&wait,wait - 1)){
                break;
            }
        }
        _sem.Signal();
    }
}