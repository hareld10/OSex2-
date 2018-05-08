
#include "Thread.h"
#include <iostream>

/**
 * Constructor
 * @param _id unique id for each thread
 * @param f the function in which the thread run
 */
Thread::Thread(int _id, void (*f)(void)) {

    address_t sp, pc;
    id = _id;
    total_quantum = 1; // init to 1
    stack = new char[STACK_SIZE]; // as the desired size
    is_blocked = false;

    if (_id == 0) { // case main thread
        pc = (address_t)0;
    }
    else
    {
        // gives the function location
        pc = (address_t) f;
    }

    // give the stack pointer position
    sp = (address_t) stack + STACK_SIZE - sizeof(address_t);
    // save state and save mask
    env = (sigjmp_buf *) new sigjmp_buf();
    int ret_val = sigsetjmp((*env), 1);
    if (ret_val == 1) {
        return;
    }

    ((*env)->__jmpbuf)[JB_SP] = translate_address(sp);
    ((*env)->__jmpbuf)[JB_PC] = translate_address(pc);
    sigemptyset(&((*env)->__saved_mask));
}


address_t Thread::translate_address(address_t addr) {
    address_t ret;
    asm volatile("xor    %%fs:0x30,%0\n"
            "rol    $0x11,%0\n"
    : "=g" (ret)
    : "0" (addr));
    return ret;
}

/**
 * add one quantum unit to total_quantum
 */
void Thread::add_one_quan() {
    total_quantum++;
}
/**
 * Destructor - deleting allocated space
 */
Thread::~Thread()
{
    delete env;
    delete stack;

}