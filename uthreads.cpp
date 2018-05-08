// Created by hareld10 on 4/24/18.
//

#ifndef _UTHREADS_CPP
#define _UTHREADS_CPP

#include "Threads.h"
#include "uthreads.h"
/*
 * User-Level Threads Library (uthreads)
 * Author: OS, os@cs.huji.ac.il
 */

#define MAX_THREAD_NUM 100 /* maximal number of threads */
#define STACK_SIZE 4096 /* stack size per thread (in bytes) */
#define FAIL_CODE (-1)
#define SUCCESS_CODE 0
int _quantum_usecs;
int total_quantum;

#define GRN "\e[32m"
#define RED "\x1B[31m"
#define RESET "\x1B[0m"
# define USEC_FACTOR 1000000



/* External interface */

/**
 *  Either blocks or unblocks all the signals according to the boolean value
 * @param block true iff we want to block all signals
 */
void signalHandler(bool block)
{
//    std::cerr<<"in signalHandler  ";
//    std::cerr<<block<<std::endl;

//    std::flush(std::cerr);
    if (block)
    {
        if (sigprocmask (SIG_BLOCK, &signals, nullptr) == FAIL_CODE)
        {
            std::cerr<<"Failed in blocking signals";
            exit(1);
        }
    }
    else
    {
        if (sigprocmask (SIG_UNBLOCK, &signals, nullptr) == FAIL_CODE)
        {
            std::cerr<<"Failed in un-blocking signals";
            exit(1);
        }
    }
}


/**
 * Switch between the running thread and the next ready thread in line
 */
void switchThreads(int sig)
{
    std::cerr<<"In Switch: ";

    signalHandler(true);
    auto currentThread = Threads::get_thread(Threads::running_thread_id());
    auto nextThread = Threads::getReadyThread();

    std::cerr<<RED "Switching from " << currentThread->id << " To "<< nextThread->id << RESET <<std::endl;
    if (nextThread == nullptr) // Ready queue is empty
    {
        currentThread->total_quantum++;
        total_quantum++;
        signalHandler(false);
        return;
    }
    if(currentThread != nullptr)
    {
        int ret_val = sigsetjmp(*(currentThread->env), 1);
        if (ret_val == 1)
        {
//            std::cerr<<"got here from jump, returning...\n";
            total_quantum++;
            currentThread->total_quantum++;
            signalHandler(false);
            return;
        }
        if(!currentThread->is_blocked) {
            Threads::add_ready(currentThread);
        }
        else{
            Threads::add_blocked(currentThread);
//            std::cerr << "Size of blocked in switch "<< Threads::_blocked_threads->size() << "\n";
        }
    }

    Threads::setRunningThread(nextThread);
    nextThread->total_quantum++;
    total_quantum++;
    resetTimer(_quantum_usecs);
    signalHandler(false);
//    std::cerr<<"id: " << nextThread->id<<std::endl;
    siglongjmp(*(nextThread->env) ,1);
}


/**
 * Resets the time count for this process when either, the quantom for a thread ends, or threads are switched due to
 * termination or blocking of the running thread.
 */
void resetTimer(int quantum_usecs)
{
//    std::cerr<<"in resetTimer"<<std::endl;
    if (sigaction(SIGVTALRM, &sa, nullptr) < 0) {
        std::cerr<< "sigaction error"<<std::endl;
    }


    timer.it_value.tv_sec = quantum_usecs / USEC_FACTOR ;
    timer.it_value.tv_usec = quantum_usecs % USEC_FACTOR ;
    timer.it_interval.tv_sec = quantum_usecs / USEC_FACTOR ;
    timer.it_interval.tv_usec = quantum_usecs % USEC_FACTOR ;

    if (setitimer(ITIMER_VIRTUAL, &timer, nullptr) == FAIL_CODE)
    {
        //if the set timer fails, print out a system call error and exit(1)
        std::cerr<<"reset fail"<<std::endl;
        exit(1);
    }
//    std::cerr<<"out resetTimer"  <<std::endl;

}


/*
 * Description: This function initializes the thread library.
 * You may assume that this function is called before any other thread library
 * function, and that it is called exactly once. The input to the function is
 * the length of a quantum in micro-seconds. It is an error to call this
 * function with non-positive quantum_usecs.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_init(int quantum_usecs)
{
    if(quantum_usecs <= 0)
    {
        std::cerr<<"line 121\n";

        return FAIL_CODE;
    }
    Thread::set_quantum_length(quantum_usecs);
    _quantum_usecs = quantum_usecs;

    // Install timer_handler as the signal handler for SIGVTALRM:
    sa.sa_handler = &switchThreads;

    sigemptyset(&signals);
    sigaddset(&signals, SIGVTALRM);


    Threads::init();
    // Create the main thread (id = 0):
    auto mainThread = new Thread(0, nullptr);
    Threads::setRunningThread(mainThread);
//    Threads::add_ready(mainThread);
    total_quantum = 1;
    resetTimer(quantum_usecs);

}

/*
 * Description: This function creates a new thread, whose entry point is the
 * function f with the signature void f(void). The thread is added to the end
 * of the READY threads list. The uthread_spawn function should fail if it
 * would cause the number of concurrent threads to exceed the limit
 * (MAX_THREAD_NUM). Each thread should be allocated with a stack of size
 * STACK_SIZE bytes.
 * Return value: On success, return the ID of the created thread.
 * On failure, return -1.
*/
int uthread_spawn(void (*f)(void)){
//    std::cerr<<"in uthread_spawn\n";

    signalHandler(true);  // Block all signals

    // check if not exceeded MAX NUM
    if(Threads::total_num_of_threads == MAX_THREAD_NUM){
        std::cerr<<"line 153\n";
        signalHandler(false);
        return FAIL_CODE;
    }
    Threads::total_num_of_threads++;

    //get next id for new Thread
    int id = Threads::get_next_id();
    auto *new_thread = new Thread(id, f);
    std::cerr<<RED "Creating thread" << id << " With ADDR " << &(*new_thread)<< RESET <<std::endl;
    // Add to ready list
    Threads::add_ready(new_thread);

    signalHandler(false);  // Free signals
    return id;
}


/*
 * Description: This function terminates the thread with ID tid and deletes
 * it from all relevant control structures. All the resources allocated by
 * the library for this thread should be released. If no thread with ID tid
 * exists it is considered as an error. Terminating the main thread
 * (tid == 0) will result in the termination of the entire process using
 * exit(0) [after releasing the assigned library memory].
 * Return value: The function returns 0 if the thread was successfully
 * terminated and -1 otherwise. If a thread terminates itself or the main
 * thread is terminated, the function does not return.
*/
int uthread_terminate(int tid)
{
    std::cerr<<RED "Terminating thread " << tid << RESET <<std::endl;

    signalHandler(true);  // Block all signals

    if(tid == 0){
        signalHandler(false);
        exit(EXIT_SUCCESS);

    }
    Thread* to_terminate = Threads::get_thread(tid);
    if (to_terminate == nullptr){
        signalHandler(false);
        return FAIL_CODE;
    }
    delete to_terminate;
//    std::vector<std::vector<int>*> ** tmp = new std::vector<int> *(Threads::syncing);
    std::vector<std::vector<int>*> tmp = *Threads::syncing;
    std::vector<int>* to_free = new std::vector<int>(*tmp[tid]);
    Threads::free_syncing_threads(tid);

    auto vec = (to_free)->begin();
    while(vec != (to_free)->end()) {
        int cur_id = *vec;
        uthread_resume(cur_id);
        vec++;
    }

    delete to_free;
    Threads::addTid(tid);  // Add the now free id to the id pool
    signalHandler(false);  // unBlock all signals
    if(Threads::running_thread_id() == tid)
    {
        switchThreads(2777);
    }
    resetTimer(_quantum_usecs);

    return SUCCESS_CODE;
}

/*
 * Description: This function blocks the thread with ID tid. The thread may
 * be resumed later using uthread_resume. If no thread with ID tid exists it
 * is considered as an error. In addition, it is an error to try blocking the
 * main thread (tid == 0). If a thread blocks itself, a scheduling decision
 * should be made. Blocking a thread in BLOCKED state has no
 * effect and is not considered as an error.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_block(int tid){
    signalHandler(true);  // Block all signals
    std::cerr<<RED "blocking thread number" << tid << RESET <<std::endl;

    if(tid == 0){
        signalHandler(false);
        std::cerr<<"trying to block main thread"<<std::endl;
        return FAIL_CODE;
    }
    if(Threads::exist_by_id_blocked(tid)){
        signalHandler(false);
        std::cerr<<"In exist_by_id_blocked"<<std::endl;
        return SUCCESS_CODE;
    }


    Thread* to_block = Threads::get_thread(tid);
    if (to_block == nullptr){
        signalHandler(false);
        return FAIL_CODE;
    }

    Threads::add_blocked(to_block);
    resetTimer(_quantum_usecs);
    signalHandler(false);  // unBlock all signals

    if(Threads::running_thread_id() == tid  ){
        switchThreads(26);
    }
    return SUCCESS_CODE;

}


/*
 * Description: This function resumes a blocked thread with ID tid and moves
 * it to the READY state. Resuming a thread in a RUNNING or READY state
 * has no effect and is not considered as an error. If no thread with
 * ID tid exists it is considered as an error.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_resume(int tid){
    std::cerr<<RED "Resuming thread " << tid << RESET <<std::endl;
    signalHandler(true);  // Block all signals

    bool t1 = Threads::running_thread_id()== tid;
    bool t2 = Threads::exist_by_id_ready(tid);
    bool t3 = Threads::is_synced(tid);
    if((t1||t2)||t3){
        signalHandler(false);
        if(t1){
            std::cerr<<"line 247- 1\n";

        }
        if(t2){
            std::cerr<<"line 247- 2\n";

        }
        if(t3){
            std::cerr<<"line 247- 3\n";

        }
        return 0;
    }
    Thread* to_resume = Threads::get_thread(tid);

    if(to_resume == nullptr){
        signalHandler(false);
        std::cerr<< "cant get thread in resume"<<std::endl;
        return FAIL_CODE;
    }

    std::cerr<<RED "RESUME::to_resume got thread with" << to_resume->id << RESET <<std::endl;

    to_resume->is_blocked = false;
    Threads::add_ready(to_resume);

    std::cerr<<RED "RESUME::Finish Added to_resume to ReadyVector " << to_resume->id << RESET <<std::endl;
    resetTimer(_quantum_usecs);
    signalHandler(false);  // unBlock all signals
    return SUCCESS_CODE;
}


/*
 * Description: This function blocks the RUNNING thread until thread with
 * ID tid will move to RUNNING state (i.e.right after the next time that
 * thread tid will stop running, the calling thread will be resumed
 * automatically). If thread with ID tid will be terminated before RUNNING
 * again, the calling thread should move to READY state right after thread
 * tid is terminated (i.e. it won’t be blocked forever). It is considered
 * as an error if no thread with ID tid exists or if the main thread (tid==0)
 * calls this function. Immediately after the RUNNING thread transitions to
 * the BLOCKED state a scheduling decision should be made.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_sync(int tid)
{
    std::cerr<<RED "Synced called by: " << Threads::running_thread_id() << " on thred" << tid << RESET <<std::endl;

    signalHandler(true);  // Block all signals
    if(Threads::running_thread_id() == tid || tid == 0){
        signalHandler(false);
        std::cerr<< "running thread calles sync"<<std::endl;
        exit(FAIL_CODE);
    }
//    Thread *to_sync = Threads::get_thread(tid);
    auto to_sync = Threads::get_thread_ptr(tid);
    if (to_sync == nullptr){
        signalHandler(false);
        std::cerr<< "to_sync thread not found"<<std::endl;
        return FAIL_CODE;
    }

    Threads::sync(tid);
    Thread *cur_running = Threads::get_running_thread();
//    cur_running->is_synced = true;
//    std::cerr<<"Size of blocked before synced"<<Threads::_blocked_threads->size()<<"\n";
//    std::cerr<<"Size of ready"<<Threads::_ready_threads->size()<<"\n";
    Threads::add_blocked(cur_running);
//    std::cerr<<"Size of blocked after synced"<<Threads::_blocked_threads->size()<<"\n";
//    std::cerr<<"Size of ready"<<Threads::_ready_threads->size()<<"\n";
//    std::cerr<<"syncing with id "<<cur_running->id<<"\n";
    signalHandler(false);  // unBlock all signals
    switchThreads(2);

}


/*
 * Description: This function returns the thread ID of the calling thread.
 * Return value: The ID of the calling thread.
*/
int uthread_get_tid() {
    return Threads::running_thread_id();
}

/*
 * Description: This function returns the total number of quantums that were
 * started since the library was initialized, including the current quantum.
 * Right after the call to uthread_init, the value should be 1.
 * Each time a new quantum starts, regardless of the reason, this number
 * should be increased by 1.
 * Return value: The total number of quantums.
*/
int uthread_get_total_quantums(){
    return total_quantum;
}

/*
 * Description: This function returns the number of quantums the thread with
 * ID tid was in RUNNING state. On the first time a thread runs, the function
 * should return 1. Every additional quantum that the thread starts should
 * increase this value by 1 (so if the thread with ID tid is in RUNNING state
 * when this function is called, include also the current quantum). If no
 * thread with ID tid exists it is considered as an error.
 * Return value: On success, return the number of quantums of the thread with ID tid. On failure, return -1.
*/
int uthread_get_quantums(int tid)
{
    return Threads::sum_by_id(tid);
}

#endif

