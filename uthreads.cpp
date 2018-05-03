// Created by hareld10 on 4/24/18.
//

#ifndef _UTHREADS_CPP
#define _UTHREADS_CPP

#include "Thread.h"
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



/* External interface */

/**
 *  Either blocks or unblocks all the signals according to the boolean value
 * @param block true iff we want to block all signals
 */
void signalHandler(bool block)
{
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
            std::cerr<<"Failed in blocking signals";
            exit(1);
        }
    }
}


/**
 * Switch between the running thread and the next ready thread in line
 */
void switchThreads(int sig)
{
    auto currentThread = Threads::get_thread(Threads::running_thread_id());
    auto nextThread = Threads::getReadyThread();
    if (nextThread == nullptr) // Ready queue is empty
    {
        return;
    }

    int ret_val = sigsetjmp(currentThread->env, 1);
    if (ret_val == 1)  //sigsetjmp failed
    {
        return;
    }
    printf("SWITCH: ret_val=%d\n", ret_val);
    Threads::setRunningThread(nextThread);
    Threads::add_ready(currentThread);
    siglongjmp(nextThread->env ,1);

    //todo What else?
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
        return FAIL_CODE;
    }
    Thread::set_quantum_length(quantum_usecs);


    // Install timer_handler as the signal handler for SIGVTALRM:
    sa.sa_handler = &switchThreads;
    if (sigaction(SIGALRM, &sa, nullptr) < 0) {
        printf("sigaction error.");
    }
    timer.it_value.tv_usec = quantum_usecs;		// first time interval, microseconds part
    timer.it_interval.tv_usec = quantum_usecs;	// following time intervals, microseconds part
    Threads::init();
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
    signalHandler(true);  // Block all signals

    // check if not exceeded MAX NUM
    if(Threads::total_num_of_threads == MAX_THREAD_NUM){
        return FAIL_CODE;
    }
    Threads::total_num_of_threads++;

    //get next id for new Thread
    int id = Threads::get_next_id();

    auto *new_thread = new Thread(id, f);
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
    signalHandler(true);  // Block all signals

    if(tid == 0){
        exit(EXIT_SUCCESS);
    }

    Threads::free_syncing_threds(tid);
    if (Threads::get_thread(tid) == nullptr){
        return FAIL_CODE;
    }

    Threads::addTid(tid);  // Add the now free id to the id pool
    signalHandler(false);  // unBlock all signals

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

    if(tid == 0){
        return FAIL_CODE;
    }
    if(Threads::exist_by_id_blocked(tid)){
        return SUCCESS_CODE;
    }

    Thread* to_block = Threads::get_thread(tid);
    if (to_block == nullptr){
        return FAIL_CODE;
    }

    Threads::add_blocked(to_block);
    signalHandler(false);  // unBlock all signals

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
    signalHandler(true);  // Block all signals

    if((Threads::running_thread_id()== tid)||(Threads::exist_by_id_ready(tid))){
        return 0;
    }
    Thread* to_resume = Threads::get_thread(tid);
    if(to_resume == nullptr){
        std::cout<< "cant get thread in resume";
        return FAIL_CODE;
    }
    Threads::add_ready(to_resume);
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
    signalHandler(true);  // Block all signals
    if(Threads::running_thread_id() == tid){
        std::cout<< "running thread calles sync";
        exit(FAIL_CODE);
    }
    Thread *to_sync = Threads::get_thread(tid);
    if (to_sync == nullptr){
        std::cout<< "to_sync thread not found";
        return FAIL_CODE;
    }

    Threads::sync(tid);
    Thread *cur_running = Threads::get_running_thread();
    signalHandler(false);  // unBlock all signals

    Threads::add_blocked(cur_running);
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
    return Threads::sum_all_usec();
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
int uthread_get_quantums(int tid) {
    return Threads::sum_by_id(tid);
}

#endif

