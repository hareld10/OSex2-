#include <iostream>
#include <list>

#include "uthreads.h"
#define SECOND 1000000


void func1(void){
    std::cout << "Func1!" << std::endl;
    std::flush(std::cout);
    for( ; ;) { }
}

void func2(void){
    std::cout << "Func2!" << std::endl;
    std::flush(std::cout);


    for( ; ;) { }
}

void func3(void){
    std::cout << "Func3!" << std::endl;
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


