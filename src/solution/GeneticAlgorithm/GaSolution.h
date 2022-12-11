//
// Created by faniche on 22-4-20.
//

#ifndef SCHEDPLUS_GA_SOLUTION_H
#define SCHEDPLUS_GA_SOLUTION_H

#include <vector>

struct TtStreams {
    std::vector<sched_time_t> offsets;
    std::vector<route_t> routes;
    bool useNoWait;
};

#endif //SCHEDPLUS_GA_SOLUTION_H
