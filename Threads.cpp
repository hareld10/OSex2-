//
// Created by mstav on 4/29/18.
//
#include "Threads.h"



int Threads::total_num_of_threads = 0;
std::priority_queue<int, std::vector<int>,  std::greater<int>> Threads::pq;
std::vector<int>*  Threads::syncing[MAX_THREAD_NUM];
std::deque<Thread*> *Threads::_ready_threads;
std::deque<Thread*> *Threads::_blocked_threads;
Thread *Threads::_running_thread;

void Threads::init() {
    _ready_threads = new std::deque<Thread*>;
    _blocked_threads = new std::deque<Thread*>;
    pq = *new std::priority_queue<int, std::vector<int>,  std::greater<int>>();
    for (int i = 1; i < MAX_THREAD_NUM; i ++)
    {
        pq.push(i);
        syncing[i] = new std::vector<int>();
    }

}

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
    _blocked_threads = nullptr;

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
    thread->is_blocked = true;
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
    std::cout<<"line135 Threads.cpp"<<std::endl;
    std::flush(std::cout);
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

//        std::cout << "Returned nullptr from running_thread_id\n";
        return 0;
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
    int sum = 1;
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

    std::cout << "tid in sum;" << tid <<std::endl;
    if(_running_thread->id == tid){
        return _running_thread->total_quantum;
    }
    std::cout << "Error in Sum by id"<<std::endl;
    return FAIL_CODE;
}

void Threads::sync(int tid) {
    syncing[tid]->push_back(running_thread_id());

}

int Threads::get_next_id() {
    int ret = Threads::pq.top();
    pq.pop();
    return ret;
}

void Threads::free_syncing_threads(int tid) {
    (*syncing[tid]).clear();

}

/**
 * returns the next ready thread and pops it from the queue
 * @return
 */
Thread* Threads::getReadyThread() {
    if (_ready_threads->empty())
    {
        return nullptr;
    }
    auto temp = _ready_threads->front();
    _ready_threads->pop_front();
    return temp;

}

void Threads::setRunningThread(Thread *thread)
{
    _running_thread = thread;

}

Thread *Threads::get_running_thread() {
    return _running_thread;
}

void Threads::addTid(int tid)
{
    pq.push(tid);
}

Thread *Threads::get_thread_ptr(int tid) {
    auto iter = _blocked_threads->begin();
    for( ; iter != _blocked_threads->end(); ++iter)
    {
        // Found thread, add it to the ready list:
        if ((*iter)->id == tid)
        {
            return *iter;
        }
    }
    iter = _ready_threads->begin();
    for( ; iter != _ready_threads->end(); ++iter)
    {
        // Found thread, add it to the ready list:
        if ((*iter)->id == tid)
        {
            return *iter;
        }
    }

    if (tid == running_thread_id())
    {
        return _running_thread;
    }
    // Thread not found:
    return nullptr;
}

bool Threads::is_synced(int tid) {
    for(auto vec: Threads::syncing){
        for(auto id: *vec){
            if(id == tid){
                return true;
            }
        }
    }
    return false;
}

