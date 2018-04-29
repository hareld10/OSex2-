//
// Created by hareld10 on 4/29/18.
//

#ifndef OSEX2_SCHEDULER_H
#define OSEX2_SCHEDULER_H


#include <vector>
#include <queue>
#include <algorithm>
#include "Thread.h"
#include "Threads.h"
#define FAIL_CODE (-1)


class Scheduler
{

public:
    static int total_num_of_threads;
    static Threads my_threads;
    static std::priority_queue<int, std::vector<int>,  std::less<int>> pq;
    static int remove_blocked_thread(int id);
    static int remove_ready_thread(int tid);


    /**
    * Removes the thread from its' vector and returns it.
    * @param tid id of wanted thread.
    * @return wanted thread if exists, nullptr o.w..
    */
    static Thread* get_thread(int tid);


    // stav needs to call that
    static void init();
    static int get_next_id();
    static void insert_id(int _id);

    static void add_ready(Thread *thread);
    static void add_blocked(Thread *thread);
};

#endif //OSEX2_SCHEDULER_H
