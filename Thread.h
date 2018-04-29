//
// Created by hareld10 on 4/29/18.
//

#ifndef OSEX2_THREAD_H
#define OSEX2_THREAD_H
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

typedef unsigned long address_t;

#define READY 0
#define RUNNING 1
#define BLOCKED 2
#define STACK_SIZE 4096
#define JB_SP 6
#define JB_PC 7

class Thread
{


public:
    explicit Thread(int _id, void (*f)(void));
    ~Thread()= default;//todo maybe implement this later
    void set_id(int id);
    static void set_quantum_length(int length);
    static address_t translate_address(address_t addr);

    int id;
    int state;
    int total_quantum;
    static int quantum_length;
    sigjmp_buf env;
    char stack[STACK_SIZE];
};


#endif //OSEX2_THREAD_H
