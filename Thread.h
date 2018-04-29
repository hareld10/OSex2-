//
// Created by hareld10 on 4/29/18.
//

#ifndef OSEX2_THREAD_H
#define OSEX2_THREAD_H
#include <sys/time.h>
#define READY 0;
#define RUNNING 1;
#define BLOCKED 2;

class Thread
{

//    static int quantum_usecs;

public:
    explicit Thread(int _id);
    void set_id(int id);


    int id;
    int state;
    time_t total_quantom;
};


#endif //OSEX2_THREAD_H
