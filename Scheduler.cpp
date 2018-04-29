//
// Created by hareld10 on 4/29/18.
//
#include "uthreads.h"
#include "Scheduler.h"

int Scheduler::total_num_of_threads = 0;
std::priority_queue<int, std::vector<int>,  std::less<int>> Scheduler::pq;
Threads Scheduler::my_threads;

void Scheduler::init(){
    for (int i = 0; i < MAX_THREAD_NUM; i ++){
        pq.push(i);
    }
    my_threads = Threads();
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

