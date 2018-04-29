//
// Created by hareld10 on 4/29/18.
//
#include "Thread.h"

int Thread::quantum_length=0;


Thread::Thread(int _id, void (*f)(void)) {
    address_t sp, pc;
    id = _id;
    sp = (address_t)stack + STACK_SIZE - sizeof(address_t);
    pc = (address_t)f;
    // save state and save mask
    sigsetjmp(env, 1);
    (env->__jmpbuf)[JB_SP] = translate_address(sp);
    (env->__jmpbuf)[JB_PC] = translate_address(pc);
    sigemptyset(&env->__saved_mask);
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
