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


public:
    explicit Thread(int _id);
    void set_id(int id);
    static void set_quantum_length(int length);


    int id;
    int state;
    int total_quantum;
    static int quantum_length;
};


#endif //OSEX2_THREAD_H
