//
// Created by mstav on 4/29/18.
//
#include <vector>

#include "Threads.h"



Threads::Threads() {
    _ready_threads = new std::vector<Thread*>;
    _blocked_threads = new std::vector<Thread*>;


}

void Threads::add_ready(Thread *thread) {
    _ready_threads->push_back(thread);

}

void Threads::add_blocked(Thread *thread) {
    _blocked_threads->push_back(thread);

}
