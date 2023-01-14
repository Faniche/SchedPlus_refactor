//
// Created by faniche on 22-4-20.
//

#ifndef SCHEDPLUS_GA_SOLUTION_H
#define SCHEDPLUS_GA_SOLUTION_H

#include <vector>
#include <map>
#include "../../type.h"

using std::map;     using std::vector;

struct TtStreams {
    vector<sched_time_t> offsets;
    vector<sched_time_t> upperBounds;
    vector<route_t> routes;
    bool useNoWait;
    map<group_id_t, vector<stream_id_t>> groupStream;
};

#endif //SCHEDPLUS_GA_SOLUTION_H
