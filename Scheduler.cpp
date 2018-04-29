//
// Created by hareld10 on 4/29/18.
//
#include "uthreads.h"
#include "Scheduler.h"

int Scheduler::total_num_of_threads = 0;
std::priority_queue<int, std::vector<int>,  std::less<int>> Scheduler::pq;
Threads Scheduler::my_threads = Threads();
std::vector<int>*  Scheduler::syncing[MAX_THREAD_NUM];


void Scheduler::init(){
    for (int i = 0; i < MAX_THREAD_NUM; i ++)
    {
        pq.push(i);
    }
    my_threads = Threads();
    Scheduler::setup_sync();  // Initiate syncing array
}

int Scheduler::get_next_id(){
    int ret = Scheduler::pq.top();
    pq.pop();
    return ret;
}

void Scheduler::insert_id(int _id){
    Scheduler::pq.push(_id);
}

void Scheduler::add_ready(Thread *thread) {
    my_threads.add_ready(thread);

}

void Scheduler::add_blocked(Thread *thread) {
    my_threads.add_blocked(thread);
}

bool Scheduler::exist_by_id_ready(int id) {
    return my_threads.exist_by_id_ready(id);

}

bool Scheduler::exist_by_id_blocked(int id) {
    return my_threads.exist_by_id_blocked(id);
}

int Scheduler::running_thread_id() {
    return my_threads.running_thread_id();
}


int Scheduler::remove_blocked_thread(int id) {
    if (id <= 0)
    {
        return FAIL_CODE;
    }
    return my_threads.remove_blocked_thread(id);
}


/**
* Removes the thread from its' vector and returns it.
* @param tid id of wanted thread.
* @return wanted thread if exists, nullptr o.w..
*/
Thread *Scheduler::get_thread(int tid) {
    return my_threads.get_thread(tid);
}


int Scheduler::remove_ready_thread(int tid) {
    if (tid <= 0)
    {
        return FAIL_CODE;
    }
    return my_threads.remove_ready_thread(tid);
}



void Scheduler::sync(int tid) {
    syncing[running_thread_id()]->push_back(tid);

}

void Scheduler::setup_sync() {
    int i=1;
    while(i < MAX_THREAD_NUM)
    {
        syncing[i] = new std::vector<int>();
        ++i;
    }

}

Scheduler::~Scheduler() {
    int i=1;
    while(i < MAX_THREAD_NUM)
    {
        delete syncing[i];
        ++i;
    }
}

