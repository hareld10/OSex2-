#include <iostream>
#include <list>

#include "uthreads.h"



void func1(void){
    std::cout << "Hello World!" << std::endl;
    std::flush(std::cout);
    for( ; ;) { }
}
void func2(void){
    std::cout << "Junk!" << std::endl;
    for( ; ;) { }
}

int main(){

    uthread_init(1);
    int id1 = uthread_spawn(func1);
    int id2 = uthread_spawn(func2);

    for( ; ; ){}
}


