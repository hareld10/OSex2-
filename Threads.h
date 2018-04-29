//
// Created by hareld10 on 4/29/18.
//

#ifndef OSEX2_THREADS_H
#define OSEX2_THREADS_H
#include <vector>
#include <sys/time.h>
#include "Thread.h"
#include <iostream>
#define FAIL_CODE (-1)


class Threads
{

public:
    explicit Threads();
    ~Threads();
    void add_ready(Thread* thread);
    void add_blocked(Thread* thread);
    int remove_blocked_thread(int id);
    int remove_ready_thread(int id);

private:

    std::vector<Thread*> *_ready_threads;
    std::vector<Thread*> *_blocked_threads;



};


#endif //OSEX2_THREADS_H
