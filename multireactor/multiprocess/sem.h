#ifndef __SEM_H__
#define __SEM_H__
#include<semaphore.h>

#include "Types.h"

#define SEM_INIT_VAL (1)
class semaphore{
public:
    semaphore(bool shared = true);
    ~semaphore();

    //noncopyable
    semaphore(const semaphore&) = delete;
    void operator=(const semaphore&) = delete;

    void Wait();
    void Signal();
    void WaitTimeout(uint32 timeout);
private:
    sem_t _sem;
};

#endif //__SEM_H__