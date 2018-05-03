//
// Created by mstav on 4/29/18.
//
#include <vector>
#include "uthreads.h"
#include "Threads.h"


int Threads::total_num_of_threads = 0;
std::priority_queue<int, std::vector<int>,  std::less<int>> Threads::pq;
std::vector<int>*  Threads::syncing[MAX_THREAD_NUM];
std::vector<Thread*> *Threads::_ready_threads;
std::vector<Thread*> *Threads::_blocked_threads;
Thread *Threads::_running_thread;

/**
 *  Destructor
 */
Threads::~Threads() {
    // Delete the ready threads vector:
    for (Thread* t:(*_ready_threads)){
        delete t;
    }
    delete _ready_threads;
    _running_thread = nullptr;

    // Delete the blocked threads vector:
    for (Thread* t:(*_blocked_threads)){
        delete t;
    }
    delete _blocked_threads;

    int i=1;
    while(i < MAX_THREAD_NUM)
    {
        delete syncing[i];
        ++i;
    }
    //todo if we allocated this instance with "new"- add "delete this"
}

void Threads::add_ready(Thread *thread) {
    _ready_threads->push_back(thread);

}

void Threads::add_blocked(Thread *thread) {
    sigsetjmp(thread->env, 1);
    _blocked_threads->push_back(thread);

}

int Threads::remove_blocked_thread(int id)
{
    auto iter = _blocked_threads->begin();
    for( ; iter != _blocked_threads->end(); ++iter)
    {
        if ((*iter)->id == id)
        {
            Thread* temp = *iter;
            _blocked_threads->erase(iter);
            delete(temp);
            return EXIT_SUCCESS;
        }
    }
    // Thread not found:
    return FAIL_CODE;
}

int Threads::remove_ready_thread(int id) {
    if (id <= 0)
    {
        return FAIL_CODE;
    }
    auto iter = _ready_threads->begin();
    for( ; iter != _ready_threads->end(); ++iter)
    {
        if ((*iter)->id == id)
        {
            Thread* temp = *iter;
            _ready_threads->erase(iter);
            delete(temp);
            return EXIT_SUCCESS;
        }
    }
    // Thread not found:
    return FAIL_CODE;
}

/**
 * Removes the thread from its' vector and returns it.
 * @param tid id of wanted thread.
 * @return wanted thread if exists, nullptr o.w..
 */
Thread *Threads::get_thread(int tid) {
    auto iter = _blocked_threads->begin();
    for( ; iter != _blocked_threads->end(); ++iter)
    {
        // Found thread, add it to the ready list:
        if ((*iter)->id == tid)
        {
            Thread* temp = *iter;
            _blocked_threads->erase(iter);
            return temp;
        }
    }
    iter = _ready_threads->begin();
    for( ; iter != _ready_threads->end(); ++iter)
    {
        // Found thread, add it to the ready list:
        if ((*iter)->id == tid)
        {
            Thread* temp = *iter;
            _ready_threads->erase(iter);
            return temp;
        }
    }

    if (tid == running_thread_id())
    {
        return _running_thread;
    }
    // Thread not found:
    return nullptr;
}

bool Threads::exist_by_id_ready(int id) {
    for(Thread* i: *_ready_threads){
        if(i->id == id){
            return true;
        }
    }
    return false;
}

int Threads::running_thread_id(){
    if (_running_thread == nullptr){
        std::cout << "Returned nullptr from running_thred_id";
        return -1;
    }
    return _running_thread->id;
}

bool Threads::exist_by_id_blocked(int id) {
    for(Thread* i: *_blocked_threads){
        if(i->id == id){
            return true;
        }
    }
    return false;
}

int Threads::sum_all_usec() {
    int sum = 0;
    for(Thread* i: *_ready_threads){
        sum+=i->total_quantum;
    }
    for(Thread* i: *_blocked_threads){
        sum+=i->total_quantum;
    }
    return sum+_running_thread->total_quantum;
}

int Threads::sum_by_id(int tid) {
    for(Thread* i: *_ready_threads){
        if(i->id == tid){
            return i->total_quantum;
        }
    }
    for(Thread* i: *_blocked_threads){
        if(i->id == tid){
            return i->total_quantum;
        }
    }

    if(_running_thread->id == tid){
        return _running_thread->total_quantum;
    }
    std::cout << "Error in Sum by id";
    return FAIL_CODE;
}

void Threads::init() {
    _ready_threads = new std::vector<Thread*>;
    _blocked_threads = new std::vector<Thread*>;
    for (int i = 0; i < MAX_THREAD_NUM; i ++)
    {
        pq.push(i);
        syncing[i] = new std::vector<int>();
    }

}

void Threads::sync(int tid) {
    syncing[running_thread_id()]->push_back(tid);
}

int Threads::get_next_id() {
    int ret = Threads::pq.top();
    pq.pop();
    return ret;
}

void Threads::free_syncing_threds(int tid) {
    (*syncing[tid]).clear();

}

