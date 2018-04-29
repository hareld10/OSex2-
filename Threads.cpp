//
// Created by mstav on 4/29/18.
//
#include <vector>

#include "Threads.h"


Threads::Threads() {
    _ready_threads = new std::vector<Thread*>;
    _blocked_threads = new std::vector<Thread*>;
    _running_thread = nullptr;

}

void Threads::add_ready(Thread *thread) {
    _ready_threads->push_back(thread);

}

void Threads::add_blocked(Thread *thread) {
    _blocked_threads->push_back(thread);

}

bool Threads::exist_by_id_ready(int id) {
    for(Thread* i: *_ready_threads){
        if(i->id == id){
            return true;
        }
    }
    return false;
}

int Threads::running_thred_id(){
    if (_running_thread == nullptr){
        std::cout << "Returned nullptr from running_thred_id";
        return nullptr;
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