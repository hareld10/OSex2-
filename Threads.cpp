

#include "Threads.h"


/**
 * Class Threads holds all the threads and handled theme, it servers as a data base
 *  Fields:
    total_num_of_threads - hold how many threads have been generated
    pq - holds a priority queue to handle the ids
    syncing - a vector holding a vector of all the threads who made sync to the
    vector in first place
    _ready_threads - holds all the ready threads
    _blocked_threads - holds all the blocked threads
    _running_thread - a pointer to the running thread
 */

int Threads::total_num_of_threads = 0;
std::priority_queue<int, std::vector<int>,  std::greater<int>> *Threads::pq;
std::vector<std::vector<int>*> *Threads::syncing;
std::deque<Thread*> *Threads::_ready_threads;
std::deque<Thread*> *Threads::_blocked_threads;
Thread *Threads::_running_thread;

/**
 * Init() initialize all the elements in class.
 */
void Threads::init() {
    _ready_threads = new std::deque<Thread*>;
    _blocked_threads = new std::deque<Thread*>;
    syncing = new std::vector<std::vector<int>*>;

    (*syncing).push_back( new std::vector<int>());
    pq = new std::priority_queue<int, std::vector<int>,  std::greater<int>>();
    for (int i = 1; i < MAX_THREAD_NUM; i ++)
    {
        /// Pushing indexes to the priority queue
        pq->push(i);
        /// Pushing new vectors to the yncing vector
        (*syncing).push_back( new std::vector<int>());
    }
}

/**
 *  Destructor - De-Allocates all the allocated memory
 */
void Threads::free() {
    // Delete the ready threads vector:
    for (Thread* t:(*_ready_threads))
    {
        delete t;
    }
    delete _ready_threads;
    _running_thread = nullptr;

    // Delete the blocked threads vector:
    for (Thread* t:(*_blocked_threads))
    {
        delete t;
    }
    delete _blocked_threads;
    _blocked_threads = nullptr;
    // Delete syncing vector:
    int i=0;
    while(i < MAX_THREAD_NUM)
    {
        delete (*syncing)[i];
        ++i;
    }
    delete syncing;
    delete _running_thread; // Delete ready vector:
    delete pq;
}

/**
 * Add thread to ready vector
 * @param thread
 */
void Threads::add_ready(Thread *thread) {
    _ready_threads->push_back(thread);

}
/**
 * Add thread to blocked vector
 * @param thread
 */
void Threads::add_blocked(Thread *thread)
{
    thread->is_blocked = true;
    _blocked_threads->push_back(thread);

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
        int is_correct = (*iter)->id;
        if ((*iter)->id == tid)
        {
            Thread* temp = *iter;
            _ready_threads->erase(iter);
            return temp;
        }
    }
    int num = running_thread_id();

    if (tid == running_thread_id())
    {
        return _running_thread;
    }
    // Thread not found:
    return nullptr;
}

/**
 * checks if vector exists in ready vector by id
 * @param id
 * @return
 */
bool Threads::exist_by_id_ready(int id) {
    for(Thread* i: *_ready_threads){
        if(i->id == id){
            return true;
        }
    }
    return false;
}

/**
 * gets the running thread id
 * @return
 */
int Threads::running_thread_id(){
    if (_running_thread == nullptr){

//        std::cout << "Returned nullptr from running_thread_id\n";
        return 0;
    }
    return _running_thread->id;
}

/**
 * checks if vector exists in blocked vector by id
 * @param id
 * @return
 */
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

/**
 * get thread total quantum by id
 * @param id
 * @return
 */
int Threads::sum_by_id(int tid) {
    if(_ready_threads == nullptr)
    {
        std::cout<<"nullptr in ready threads"<<std::endl;
    }
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
    std::cout << "Error in Sum by id"<<std::endl;
    return FAIL_CODE;
}

/**
 * sync the tid to the running thread
 * @param tid
 */
void Threads::sync(int tid) {
    int tmp = running_thread_id();
    (*syncing)[tid]->push_back(tmp);

}

/**
 * get next id to use for generated thread
 * @return
 */
int Threads::get_next_id() {
    int ret = Threads::pq->top();
    pq->pop();
    return ret;
}

/**
 * free syncing vector
 * @param tid
 */
void Threads::free_syncing_threads(int tid) {
    (*(*syncing)[tid]).clear();
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

/**
 * set the running thread
 * @param thread
 */
void Threads::setRunningThread(Thread *thread)
{
    _running_thread = thread;

}

/**
 * get the pointer to the running thread
 * @return
 */
Thread *Threads::get_running_thread() {
    return _running_thread;
}

/**
 * add id to the priority queue of the ids
 * @param tid
 */
void Threads::addTid(int tid)
{
    pq->push(tid);
}

/**
 * get pointer to the thread by id
 * @param tid
 * @return
 */
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

/**
 * checks if thread is synced or not
 * @param tid
 * @return
 */
bool Threads::is_synced(int tid) {
    auto vec = Threads::syncing->begin();
    while(vec != Threads::syncing->end()) {
        auto id = (*vec)->begin();
        while (id != (*vec)->end()) {
            if (*id == tid) {
                return true;
            }
            id++;
        }
        vec++;
    }
    return false;
}