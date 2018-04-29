//
// Created by hareld10 on 4/29/18.
//

#ifndef OSEX2_THREADS_H
#define OSEX2_THREADS_H
#include <vector>
#include <sys/time.h>
#include "Thread.h"
#include <iostream>



class Threads
{

public:
    explicit Threads();
    void add_ready(Thread* thread);
    void add_blocked(Thread* thread);




private:

    std::vector<Thread*> *_ready_threads;
    std::vector<Thread*> *_blocked_threads;


};


#endif //OSEX2_THREADS_H
