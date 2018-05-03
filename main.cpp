#include <iostream>
#include <list>

#include "uthreads.h"



void func1(void){
    std::cout << "Junk!" << std::endl;
    std::flush(std::cout);
    for( ; ;) { }
}

void func2(void){
    std::cout << "Hello hello hello hello" << std::endl;
    std::flush(std::cout);


    for( ; ;) { }
}

void func3(void){
    std::cout << "Hello World!" << std::endl;
    std::flush(std::cout);


    for( ; ;) { }
}

int main(){
    uthread_init(3);
    int id1 = uthread_spawn(func1);
    int id2 = uthread_spawn(func2);
    int id3 = uthread_spawn(func3);

    for( ; ; ){}
}


