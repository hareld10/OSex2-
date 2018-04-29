//
// Created by hareld10 on 4/29/18.
//
#include "Thread.h"
int Thread::quantum_length=0;


Thread::Thread(int _id) {
    id = _id;
}

void Thread::set_id(int _id) {
    id = _id;
}

void Thread::set_quantum_length(int length) {
    quantum_length = length;
}
