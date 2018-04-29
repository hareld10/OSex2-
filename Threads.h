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

    /**
     * Removes the thread from its' vector and returns it.
     * @param tid id of wanted thread.
     * @return wanted thread if exists, nullptr o.w..
     */
    Thread* get_thread(int tid);


    bool exist_by_id_ready(int id);
    bool exist_by_id_blocked(int id);
    int running_thread_id();

private:
    Thread *_running_thread;
    std::vector<Thread*> *_ready_threads;
    std::vector<Thread*> *_blocked_threads;



};


#endif //OSEX2_THREADS_H
