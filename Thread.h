////
//// Created by hareld10 on 4/29/18.
////
//
//#ifndef OSEX2_THREAD_H
//#define OSEX2_THREAD_H
//#include <stdio.h>
//#include <setjmp.h>
//#include <signal.h>
//#include <unistd.h>
//#include <sys/time.h>
//
//typedef unsigned long address_t;
//
//#define READY 0
//#define RUNNING 1
//#define BLOCKED 2
//#define STACK_SIZE 4096
//#define JB_SP 6
//#define JB_PC 7
//
//class Thread
//{
//
//
//public:
//    explicit Thread(int _id, void (*f)(void));
//    ~Thread();
//    void set_id(int id);
//    static void set_quantum_length(int length);
//    static address_t translate_address(address_t addr);
//
//    bool is_blocked;
//    bool is_synced;
//    int id;
//    int state;
//    int total_quantum;
//    static int quantum_length;
//    void add_one_quan();
//    sigjmp_buf *env;
//    char *stack;
//};
//
//
//#endif //OSEX2_THREAD_H
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

#define STACK_SIZE 4096
#define JB_SP 6
#define JB_PC 7
/**
 * Class Thread hold an object represent the thread
 * Fields:
    bool is_blocked - marks if the thread is blocked
    int id - hold the thread uniqe id
    int total_quantum - holds how many quantums the thread run
    void add_one_quan() - function to add one unit of quantum to the total quantum
    sigjmp_buf *env - holds the environment of the thread
    char *stack - holds the memory segment in which the thread runs
 */
class Thread
{

public:
    explicit Thread(int _id, void (*f)(void));
    ~Thread();
    static address_t translate_address(address_t addr);

    bool is_blocked;
    int id;
    int total_quantum;
    void add_one_quan();
    sigjmp_buf *env;
    char *stack;
};


#endif //OSEX2_THREAD_H