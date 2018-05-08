#include <iostream>
#include <list>

#include "uthreads.h"
#define SECOND 1000000


void func1(void){
//    std::cout << "Func1!" << std::endl;
    std::flush(std::cout);
    for( ; ;) { }
}

void func2(void){

//    std::cout << "Func2!" << std::endl;
    std::flush(std::cout);


    for( ; ;) { }
}

void func3(void){
//    std::cout << "Func3!" << std::endl;
    std::flush(std::cout);


    for( ; ;) { }
}

int main(){
    uthread_init(3);
    int id1 = uthread_spawn(func1);
    int id2 = uthread_spawn(func2);
    int id3 = uthread_spawn(func3);

    for( ; ; ){

    }
}


////#include <iostream>
////#include <list>
////
////#include "uthreads.h"
////#define SECOND 1000000
////#define GRN "\e[32m"
////#define RED "\x1B[31m"
////#define RESET "\x1B[0m"
////
////void error()
////{
////    printf(RED "ERROR - wrong id returned\n" RESET);
////    exit(1);
////}
////
////void thread1(void){
////    std::cout << "Func1!" << std::endl;
////    int tmp = uthread_get_tid();
//////    uthread_block(tmp);
////    for (;;);
////}
////
////void thread2(void){
////    std::cout << "Func2!" << std::endl;
////    std::flush(std::cout);
////
////
////    for( ; ;) { }
////}
////
////void func3(void){
////    std::cout << "Func3!" << std::endl;
////    std::flush(std::cout);
////
////
////    for( ; ;) { }
////}
////
////void wait_next_quantum()
////{
////    int quantum = uthread_get_quantums(uthread_get_tid());
////    while (uthread_get_quantums(uthread_get_tid()) == quantum)
////    {}
////    return;
////}
////
////
////int main(){
////    std::cout << GRN "Test 1:    " RESET <<std::endl;
////    uthread_init(10);
////    if (uthread_spawn(thread1) != 1)
////        error();
////    if (uthread_spawn(thread2) != 2)
////        error();
////    if (uthread_spawn(thread2) != 3)
////        error();
////    if (uthread_spawn(thread1) != 4)
////        error();
////    if (uthread_spawn(thread2) != 5)
////        error();
////    if (uthread_spawn(thread1) != 6)
////        error();
////
////    uthread_terminate(5);
////    if (uthread_spawn(thread1) != 5)
////        error();
////
////
////    wait_next_quantum();
////    wait_next_quantum();
////
////    uthread_terminate(5);
////    if (uthread_spawn(thread1) != 5)
////        error();
////
////    uthread_terminate(2);
////    if (uthread_spawn(thread2) != 2)
////        error();
////
////    uthread_terminate(3);
////    uthread_terminate(4);
////    if (uthread_spawn(thread2) != 3)
////        error();
////    if (uthread_spawn(thread1) != 4)
////        error();
////
////    printf(GRN "SUCCESS\n" RESET);
////    uthread_terminate(0);
////
////}
//
/////**********************************************
//// * Test 1: correct threads ids
//// *
//// **********************************************/
////
////#include <cstdio>
////#include <cstdlib>
////#include <iostream>
////#include "uthreads.h"
////
////#define GRN "\e[32m"
////#define RED "\x1B[31m"
////#define RESET "\x1B[0m"
////
////void halt()
////{
////    std::cout<<RED "halt" RESET <<std::endl;
////
////    while (true)
////    {}
////}
////
////void wait_next_quantum()
////{
////    int quantum = uthread_get_quantums(uthread_get_tid());
////    std::cout<<RED "getting quantums........" RESET <<std::endl;
////
////
////    while (uthread_get_quantums(uthread_get_tid()) == quantum)
////    {
////
////    }
////    std::cout<<RED "got quantums" RESET <<std::endl;
////
////    return;
////}
////
////void thread1()
////{
////    int tid = uthread_get_tid();
////    uthread_block(tid);
////    std::cout<<RED "func1" RESET <<std::endl;
////}
////
////void thread2()
////{
////    std::cout<<RED "func2" RESET <<std::endl;
////    halt();
////
////}
////
////void error()
////{
////    std::cout<<RED "ERROR - wrong id returned\n";
////    std::flush(std::cout);
////    exit(1);
////}
////
////int main()
////{
////    std::cout<< GRN "Test 1:    \n" RESET;
////    std::flush(std::cout);
////
////    uthread_init(10);
////    if (uthread_spawn(thread1) != 1)
////        error();
////    if (uthread_spawn(thread2) != 2)
////        error();
////    if (uthread_spawn(thread2) != 3)
////        error();
////    if (uthread_spawn(thread1) != 4)
////        error();
////    if (uthread_spawn(thread2) != 5)
////        error();
////    if (uthread_spawn(thread1) != 6)
////        error();
////
////    uthread_terminate(5);
////    if (uthread_spawn(thread1) != 5)
////        error();
////
////    wait_next_quantum();
////    std::cout<<RED "here" RESET <<std::endl;
////    wait_next_quantum();
////
////    uthread_terminate(5);
////    if (uthread_spawn(thread1) != 5)
////        error();
////
////    uthread_terminate(2);
////    if (uthread_spawn(thread2) != 2)
////        error();
////
////    uthread_terminate(3);
////    uthread_terminate(4);
////    if (uthread_spawn(thread2) != 3)
////        error();
////    if (uthread_spawn(thread1) != 4)
////        error();
////
////    std::cout<< GRN "SUCCESS\n";
////    std::flush(std::cout);
////
////    uthread_terminate(0);
////
////}
////
////
//
//
//
//
///**********************************************
// * Test 1: correct threads ids
// *
// **********************************************/
//
//#include <cstdio>
//#include <cstdlib>
//#include <iostream>
//#include "uthreads.h"
//#include <iostream>
//
//#define GRN "\e[32m"
//#define RED "\x1B[31m"
//#define RESET "\x1B[0m"
//
//void halt()
//{
//    while (true)
//    {}
//}
//
//void wait_next_quantum()
//{
//
//    int quantum = uthread_get_quantums(uthread_get_tid());
//    std::cout<< GRN"Quantum isssss:" RESET<< quantum<<std::endl;
//    std::cout<< GRN"TotalQuantum isssss:" RESET<< uthread_get_total_quantums()<<std::endl;
//    while (uthread_get_quantums(uthread_get_tid()) == quantum)
//    {
////        std::cout<< GRN"Quantum isssss:" RESET<< uthread_get_quantums(uthread_get_tid())<<std::endl;
//    }
//    std::cout<< GRN"Quantum isssss:" RESET<< uthread_get_quantums(uthread_get_tid())<<std::endl;
//    return;
//}
//
//void thread1()
//{
//    uthread_block(uthread_get_tid());
//    for(;;);
//}
//
//void thread2()
//{
//    halt();
//}
//
//void error()
//{
//    std::cout<< "ERROR - wrong id returned\n";
//    std::flush(std::cout);
//    exit(1);
//}
//
//int main()
//{
//    std::cout<< GRN "Test 1:    \n"RESET;
//    std::flush(std::cout);
//
//    uthread_init(10);
//    if (uthread_spawn(thread1) != 1)
//        error();
//    if (uthread_spawn(thread2) != 2)
//        error();
//    if (uthread_spawn(thread2) != 3)
//        error();
//    if (uthread_spawn(thread1) != 4)
//        error();
//    if (uthread_spawn(thread2) != 5)
//        error();
//    if (uthread_spawn(thread1) != 6)
//        error();
//
//    uthread_terminate(5);
//    if (uthread_spawn(thread1) != 5)
//        error();
//
//    wait_next_quantum();
//    wait_next_quantum();
//
//    uthread_terminate(5);
//    if (uthread_spawn(thread1) != 5)
//        error();
//
//    uthread_terminate(2);
//    if (uthread_spawn(thread2) != 2)
//        error();
//
//    uthread_terminate(3);
//    uthread_terminate(4);
//    if (uthread_spawn(thread2) != 3)
//        error();
//    if (uthread_spawn(thread1) != 4)
//        error();
//
//    std::cout<< GRN "SUCCESS\n";
//    std::flush(std::cout);
//
//    uthread_terminate(0);
//
//}
/**********************************************
 * Test 2: very simple sync check
 *
 **********************************************/

#include <cstdio>
#include <cstdlib>
#include "uthreads.h"

#define GRN "\e[32m"
#define RED "\x1B[31m"
#define RESET "\x1B[0m"

bool thread2Spawned = false;
bool thread2Executed = false;

void halt()
{
//    while (true)
//    {}
    for(;;);
}

void thread1()
{
    while (!thread2Spawned)
    {}

    uthread_sync(2);
    if (!thread2Executed)
    {
        printf(RED "ERROR - thread did not wait to sync\n" RESET);
        exit(1);
    }
    printf(GRN "SUCCESS\n" RESET);
    uthread_terminate(0);

}

void thread2()
{
    thread2Executed = true;
    halt();
}

int main()
{
    printf(GRN "Test 2:    " RESET);
    fflush(stdout);

    uthread_init(20);
    uthread_spawn(thread1);
    uthread_spawn(thread2);
    thread2Spawned = true;
    for(int i = 0; i < 100000000; i++){
    }
    uthread_terminate(2);
    halt();
}
