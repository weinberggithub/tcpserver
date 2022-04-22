
/*
 * process mutex inspired by nginx.
 */


#ifndef __SHMTX_H__
#define __SHMTX_H__

#include<atomic>
#include<memory>

#include "sem.h"

using uint32 = unsigned int;

class shmtx /*: public std::enable_shared_from_this<shmtx>*/{
public:
    shmtx();
    ~shmtx();
    // std::shared_ptr<shmtx> getptr() {
    //     return shared_from_this();
    // }

    void Lock();
    bool TryLock();
    void Unlock();

private:
    volatile atomic_int*    _locker;
    volatile atomic_int*    _wait;
    //not impl
    semaphore               _sem;
};

#endif /* __SHMTX_H__ */
