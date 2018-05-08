//
// Created by hareld10 on 4/29/18.
//
#include "Thread.h"


#include <iostream>


int Thread::quantum_length=0;


Thread::Thread(int _id, void (*f)(void)) {

    address_t sp, pc;
    id = _id;
    total_quantum = 1;
    stack = new char[STACK_SIZE];
    is_blocked = false;
    is_synced = false;

    if (_id == 0) {
        pc = (address_t)0;
//        sp = (address_t)0;
    }
    else
    {
        pc = (address_t) f;
    }
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

void Thread::set_id(int _id) {
    id = _id;
}

void Thread::set_quantum_length(int length) {
    quantum_length = length;
}

address_t Thread::translate_address(address_t addr) {
    address_t ret;
    asm volatile("xor    %%fs:0x30,%0\n"
            "rol    $0x11,%0\n"
    : "=g" (ret)
    : "0" (addr));
    return ret;
}

void Thread::add_one_quan() {
    total_quantum++;
}

