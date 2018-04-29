//
// Created by hareld10 on 4/29/18.
//
#include "uthreads.h"

static void Scheduler::init(){
    for (int i = 0; i < MAX_THREAD_NUM; i ++){
        pq.push(i);
    }
}

static int Scheduler::get_next_id(){
    return pq.pop();
}

static void Scheduler::insert_id(int _id){
    pq.push(_id);
}

