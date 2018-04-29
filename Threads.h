//
// Created by hareld10 on 4/29/18.
//

#ifndef OSEX2_THREADS_H
#define OSEX2_THREADS_H
#include <vector>
#include <sys/time.h>
#include "Thread.h"
#include <iostream>

#define READY 0;
#define RUNNING 1;
#define BLOCKED 2;


class Threads
{

//    static int quantum_usecs;

public:
    explicit Threads();
    int add_ready(Thread* thread);
    int add_blocked(Thread* thread);




private:

    std::vector<Thread*> *_ready_threads;
    std::vector<Thread*> *_blocked_threads;


};


#endif //OSEX2_THREADS_H
