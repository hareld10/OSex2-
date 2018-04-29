//
// Created by hareld10 on 4/29/18.
//

#ifndef OSEX2_SCHEDULER_H
#define OSEX2_SCHEDULER_H


class Scheduler
{

public:
    static priority_queue<int, vector<int>,  less<int>> pq;

    // stav needs to call that
    static void init();
    static int get_next_id();
    static void insert_id(int _id);

};

#endif //OSEX2_SCHEDULER_H
